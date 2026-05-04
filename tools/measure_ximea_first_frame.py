#!/usr/bin/env python3
"""
Measure Ximea camera open + first-frame latency for each camera by serial.

This mirrors what bb_imgacquisition does at startup (xiOpenDeviceBy(SN) +
xiStartAcquisition + xiGetImage loop) but with a GENEROUS per-call timeout
so first-frame latency is visible instead of being hidden by an error.

Usage:
    pip install ximea --break-system-packages     # or use a venv
    python3 tools/measure_ximea_first_frame.py CUMAU2252016 CUMAU2306009

If run with no args, it enumerates all attached Ximea cameras and measures
each one.

What to look for:
    * open_device_by_SN  — usually 100–500 ms; >2 s means USB/firmware issue
    * start_acquisition  — usually <100 ms
    * frame #1           — THE number you care about. On a cold open this
                           can be 2–6 s on USB3 Ximea cameras. If it's
                           consistently >2000 ms, the hard-coded timeout in
                           XimeaCamera.cpp:311 is why bb_imgacquisition is
                           aborting on the first grab.
    * frames #2..N       — should be at ~1/framerate (e.g. ~35 ms @ 28 fps).
                           If these are also >1 s, it's a bandwidth /
                           acquisition problem, not a warm-up problem.
"""

import sys
import time
import argparse

try:
    from ximea import xiapi
except ImportError:
    sys.stderr.write(
        "error: python 'ximea' package not installed.\n"
        "       Run: pip install ximea --break-system-packages\n"
        "       (or activate the environment that has it — it's bundled\n"
        "       with the Ximea Linux SDK under /opt/XIMEA/python/).\n"
    )
    sys.exit(1)


def enumerate_serials():
    """Return a list of serial numbers currently attached."""
    serials = []
    probe = xiapi.Camera()
    n = probe.get_number_devices()
    for i in range(n):
        c = xiapi.Camera(dev_id=i)
        try:
            c.open_device()
            serials.append(c.get_device_sn().decode() if isinstance(
                c.get_device_sn(), bytes) else c.get_device_sn())
        finally:
            try:
                c.close_device()
            except Exception:
                pass
    return serials


def measure(serial, warmup_frames=10, per_frame_timeout_ms=10000):
    print(f"\n=== Camera SN={serial} ===")
    cam = xiapi.Camera()
    img = xiapi.Image()

    t0 = time.perf_counter()
    cam.open_device_by_SN(serial)
    dt = (time.perf_counter() - t0) * 1000
    print(f"  open_device_by_SN     : {dt:8.1f} ms")

    # Don't try to match the full bb_imgacquisition config — we just want to
    # see the generic open/start/first-frame latency. Leave resolution and
    # exposure at defaults to keep this script plug-and-play.
    cam.set_imgdataformat('XI_MONO8')

    # Match the app's style: FREE_RUN timing, reasonable buffer count.
    try:
        cam.set_acq_timing_mode('XI_ACQ_TIMING_MODE_FREE_RUN')
    except xiapi.Xi_error:
        pass  # some params aren't settable on every firmware
    try:
        cam.set_buffers_queue_size(cam.get_buffers_queue_size_maximum())
    except xiapi.Xi_error:
        pass

    t0 = time.perf_counter()
    cam.start_acquisition()
    dt = (time.perf_counter() - t0) * 1000
    print(f"  start_acquisition     : {dt:8.1f} ms")

    try:
        for i in range(warmup_frames):
            t0 = time.perf_counter()
            cam.get_image(img, timeout=per_frame_timeout_ms)
            dt = (time.perf_counter() - t0) * 1000
            marker = "  <-- FIRST FRAME" if i == 0 else ""
            print(f"  frame #{i+1:2d} get_image  : {dt:8.1f} ms{marker}")
    finally:
        cam.stop_acquisition()
        cam.close_device()


def main():
    p = argparse.ArgumentParser()
    p.add_argument(
        "serials", nargs="*",
        help="Camera serials to test (e.g. CUMAU2252016). "
             "If omitted, all attached Ximea cameras are measured.",
    )
    p.add_argument("--warmup-frames", type=int, default=10)
    p.add_argument("--per-frame-timeout-ms", type=int, default=10000,
                   help="xiGetImage timeout per call (default 10000 ms). "
                        "bb_imgacquisition currently uses 2000 ms.")
    args = p.parse_args()

    serials = args.serials or enumerate_serials()
    if not serials:
        print("No Ximea cameras found.", file=sys.stderr)
        sys.exit(2)

    print(f"Measuring {len(serials)} camera(s): {', '.join(serials)}")
    print(f"Per-frame timeout: {args.per_frame_timeout_ms} ms, "
          f"warm-up frames: {args.warmup_frames}")

    for sn in serials:
        try:
            measure(sn,
                    warmup_frames=args.warmup_frames,
                    per_frame_timeout_ms=args.per_frame_timeout_ms)
        except xiapi.Xi_error as e:
            print(f"  ERROR for {sn}: {e}")


if __name__ == "__main__":
    main()
