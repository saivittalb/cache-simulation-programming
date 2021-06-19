/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Utility.h
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "Cache.h"

class Utility {
private:

public:
	unsigned long cacheSize;
	CacheType cacheType;
	unsigned long blockSize;
	unsigned int nWay;
	ReplacementAlgorithm replacementAlgorithm;
	WritePolicy writePolicy;
	char *fileName;

	bool isSetFileName;

	Utility();
	void showUsage(char *programName);
	void tokenizeArgv(int argc, char *argv[]);
};

#endif
