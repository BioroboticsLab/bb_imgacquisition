#!/usr/bin/env bash
# ------------------------------------------------------------
# setup_bbimgacq_service
#   Create & enable a systemd service for a given bb_imgacquisition
#   config file.  Assumes:
#     • this script lives in the project root
#     • binary is   ./build/bb_imgacquisition
#     • config is   ./<CONFIG>.json         (argument)
#
# Usage (as root):  setup_bbimgacq_service <config.json>
# Example:          setup_bbimgacq_service config_cam1.json
# ------------------------------------------------------------

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <config.json>" >&2
    exit 1
fi

CONFIG_FILE="$1"

# Determine the real user invoking sudo, or fallback to current
if [[ -n "${SUDO_USER-}" && "${SUDO_USER-}" != "root" ]]; then
    SERVICE_USER="$SUDO_USER"
else
    SERVICE_USER="$(whoami)"
fi
SERVICE_GROUP="$SERVICE_USER"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_PATH="${SCRIPT_DIR}/build/bb_imgacquisition"
CONFIG_PATH="${SCRIPT_DIR}/${CONFIG_FILE}"
SERVICE_NAME="bb_imgacq_$(basename "$CONFIG_FILE" .json).service"
SERVICE_PATH="/etc/systemd/system/${SERVICE_NAME}"
LOGDIR="${SCRIPT_DIR}/logs"

# — sanity checks —
[[ -x "$BIN_PATH"      ]] || { echo "Binary not found: $BIN_PATH" >&2; exit 1; }
[[ -f "$CONFIG_PATH"   ]] || { echo "Config not found: $CONFIG_PATH" >&2; exit 1; }

# make sure log directory exists
mkdir -p "$LOGDIR"

# — write the unit file —
cat >"$SERVICE_PATH" <<EOF
[Unit]
Description=bb_imgacquisition (${CONFIG_FILE})
After=network.target

[Service]
Type=simple
WorkingDirectory=${SCRIPT_DIR}
ExecStart=${BIN_PATH} -c ${CONFIG_PATH}
Nice=-20
# always, not on-failure: the watchdog exits 0 on camera timeout
Restart=always
RestartSec=30
# ---- safety check in case of memory leak ----
MemoryHigh=3G
MemoryMax=4G
# -----------------------------------
User=${SERVICE_USER}
Group=${SERVICE_GROUP}
StandardOutput=append:${LOGDIR}/${CONFIG_FILE%.json}.log
StandardError=append:${LOGDIR}/${CONFIG_FILE%.json}.log
RestartSec=30
TimeoutStopSec=40
KillMode=control-group

[Install]
WantedBy=multi-user.target
EOF

# — enable & start —
systemctl daemon-reload

echo "✔ Service '${SERVICE_NAME}' created."
echo "  • Start with with:  sudo systemctl start $SERVICE_NAME"
echo "  • Check status with:  systemctl status $SERVICE_NAME"
echo "  • View logs via:      journalctl -u $SERVICE_NAME -f"