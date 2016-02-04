/*
 * MutexBuffer.h
 *
 *  Created on: Jan 25, 2016
 *      Author: hauke
 */

#ifndef MUTEXBUFFER_H_
#define MUTEXBUFFER_H_

#include <mutex>
#include <string>

namespace beeCompress {

class ImageBuffer {
public:
	std::string timestamp;
	int width;
	int height;
	int camid;
	uint8_t *data;

	ImageBuffer(int w, int h, int cid, std::string t){
		timestamp = t;
		height = h;
		width = w;
		camid = cid;
		if (w>0 && h>0){
			data = (uint8_t*) malloc (w*h);
			//TODO malloc data ok?
		}
	}

	ImageBuffer(const beeCompress::ImageBuffer &b){
		timestamp = b.timestamp;
		height = b.height;
		width = b.width;
		camid = b.camid;
		data = b.data;
	}

	~ImageBuffer(){
		//free(data);
	}
};

/* LIFO Queue */
class MutexBuffer {
public:

	virtual void push(ImageBuffer imbuffer) = 0;

	virtual ImageBuffer pop() = 0;

	virtual int size() = 0;

	MutexBuffer(){};

	virtual ~MutexBuffer(){};
};

} /* namespace beeCompress */

#endif /* MUTEXBUFFER_H_ */
