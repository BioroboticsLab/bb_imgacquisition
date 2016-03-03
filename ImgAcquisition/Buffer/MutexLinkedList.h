/*
 * MutexLinkedList.h
 *
 *  Created on: Jan 25, 2016
 *      Author: hauke
 */

#ifndef MUTEXLINKEDLIST_H_
#define MUTEXLINKEDLIST_H_

#include "MutexBuffer.h"
#include <list>
#include <memory>

namespace beeCompress {

class MutexLinkedList: public MutexBuffer {

	//struct Node{
	//	ImageBuffer* imb;
	//	Node* next;
	//};

public:

	std::list<std::shared_ptr<ImageBuffer>> images;

	/**
	 * @brief _Access Mutex to modify the ringbuffer.
	 */
	std::mutex _Access;

	virtual void push(std::shared_ptr<ImageBuffer> imbuffer);//std::shared_ptr<ImageBuffer>

	virtual std::shared_ptr<beeCompress::ImageBuffer> pop();

	virtual int size(){
		int tsize = 0;
		_Access.lock();
		tsize = images.size();
		_Access.unlock();
		return tsize;
	}

	MutexLinkedList();

	virtual ~MutexLinkedList();
};

} /* namespace beeCompress */

#endif /* MUTEXLINKEDLIST_H_ */