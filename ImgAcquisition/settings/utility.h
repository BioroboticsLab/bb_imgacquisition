/*
 * utility.h
 *
 *  Created on: Nov 11, 2015
 *      Author: hauke
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>

std::string get_utc_time();
std::string get_utc_offset_string();
std::string getTimestamp();
void slackpost(std::string what, int level);

#endif /* UTILITY_H_ */
