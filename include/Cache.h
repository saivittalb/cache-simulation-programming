/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Cache.h
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#ifndef CACHE_H
#define CACHE_H

// Enum declaration
enum CacheType {
	DIRECT_MAPPED,
	ASSOCIATIVITY
};
enum ReplacementAlgorithm {
	LEAST_RECENTLY_USED,
	LEAST_FREQUENTLY_USED,
	CLOCK,
	RANDOM
};
enum WritePolicy { 
	WRITE_THROUGH,
	WRITE_ALLOCATE
};

class Cache {
// Output variables declaration
protected:
	long hit, miss, read, write;
	long nAccess;
	unsigned int wordSize;

public:
	Cache();
	virtual void access(unsigned long addr) = 0;

	// TODO: Remove printStatus (for test only)
	virtual void printStatus() = 0;
	long getHit() const;
	long getMiss() const;
	long getRead() const;
	long getWrite() const;
	long getNAccess() const;
};

#endif
