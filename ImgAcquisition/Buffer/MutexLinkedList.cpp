/*
 * MutexLinkedList.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: hauke
 */

#include "MutexLinkedList.h"
#include <iostream>

namespace beeCompress {

MutexLinkedList::MutexLinkedList() {
	// Nothing to do here: Auto-generated constructor stub
}

MutexLinkedList::~MutexLinkedList() {
	// Nothing to do here: Auto-generated destructor stub
}


void MutexLinkedList::push(std::shared_ptr<ImageBuffer> imbuffer){
	_Access.lock();

	images.push_back(std::move(imbuffer));

	_Access.unlock();
	waiting.notify();
}

std::shared_ptr<beeCompress::ImageBuffer> MutexLinkedList::pop(){
	waiting.wait();
	_Access.lock();
	if(images.size()>0){
		std::shared_ptr<ImageBuffer> img = std::move(images.front());
		images.pop_front();
		_Access.unlock();
		return img;
	}
	_Access.unlock();

	std::cout << "Warning: pop used on an empty buffer"<<std::endl;
	std::shared_ptr<ImageBuffer> dummy(new beeCompress::ImageBuffer(0,0,0,""));
	return dummy;
}

} /* namespace beeCompress */
