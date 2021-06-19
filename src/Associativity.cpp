/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Associativity.cpp
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#include <vector>
#include <cstdio>

#include "Associativity.h"
#include "Cache.h"
#include "math.h"

using namespace std;
#define BYTE_OFFSET 2
#define STARTING_AGE 0
#define DATA_SIZE 32

Associativity::Associativity(unsigned int nways, ReplacementAlgorithm replacementAlgo, WritePolicy writePolicy, unsigned long cacheSize ) {
    unsigned long entries = cacheSize / (nways * this->wordSize);

    this->nways = nways;
    this->age = STARTING_AGE;
    this->byteOffsetLength = BYTE_OFFSET;
    this->indexLength = log2(entries);
    this->tagLength = DATA_SIZE - BYTE_OFFSET - log2(entries);
    this->replacementAlgo = replacementAlgo;
    this->writePolicy = writePolicy;
    caches = vector<vector<CacheEntries>>(nways,vector<CacheEntries>(entries));

    switch (replacementAlgo)
    {
    case ReplacementAlgorithm::LEAST_RECENTLY_USED :
        break;
    case ReplacementAlgorithm::LEAST_FREQUENTLY_USED :
        break;
    case ReplacementAlgorithm::CLOCK :
        this->roundIndex = new unsigned int[entries];
        break;
    case ReplacementAlgorithm::RANDOM :
        this->randomIndex = new unsigned int[entries];
        break;
    default:
        break;
    }
}

void Associativity::access(unsigned long addr) {
    unsigned long tag, index, byteOffset;
    this->tokenizeAddr(addr ,&tag, &index, &byteOffset);
    if (this->replacementAlgo == ReplacementAlgorithm::CLOCK) {
        bool isHit = false;
        bool isInvalid = false;
        for (int way = 0 ; way < nways ; way++) {
            if (!caches[way][index].valid) {
                caches[way][index].tag = tag;
                caches[way][index].valid = true;
                isInvalid = true;
                break;
            } else if (caches[way][index].tag == tag && caches[way][index].valid) {
                isHit = true;
                break;
            }
        }
		if (!isHit && isInvalid) {
			++miss;
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
		} else if (!isHit) {
			unsigned long replaceWay = roundIndex[index];
			caches[replaceWay][index].tag = tag;
			roundIndex[index] = (roundIndex[index] + 1) % nways;
			++miss;
            if (caches[replaceWay][index].dirty) {
                ++write;
                caches[replaceWay][index].dirty = false;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++read;
                ++write;
            } else {
                ++read;
                caches[replaceWay][index].dirty = true;
            }
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
		} else {
            ++hit;
            unsigned long replaceWay = roundIndex[index];
            roundIndex[index] = (roundIndex[index] + 1) % nways;
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++write;
            } else {
                caches[replaceWay][index].dirty = true;
            }
            ++read;
		}
    } else if (this->replacementAlgo == ReplacementAlgorithm::RANDOM) {
        bool isHit = false;
        bool isInvalid = false;
        for (int way = 0 ; way < nways ; way++) {
            if (!caches[way][index].valid) {
                caches[way][index].tag = tag;
                caches[way][index].valid = true;
                isInvalid = true;
                break;
            } else if (caches[way][index].tag == tag && caches[way][index].valid) {
                isHit = true;
                break;
            }
        }
		if (!isHit && isInvalid) {
			++miss;
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
		} else if (!isHit) {
            srand(time(NULL));
            int rndIndex = rand() % (index + 1);
			unsigned long replaceWay = randomIndex[rndIndex];
			caches[replaceWay][index].tag = tag;
			randomIndex[rndIndex] = (randomIndex[rndIndex] + 1) % nways;
			++miss;
            if (caches[replaceWay][rndIndex].dirty) {
                ++write;
                caches[replaceWay][rndIndex].dirty = false;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++read;
                ++write;
            } else {
                ++read;
                caches[replaceWay][rndIndex].dirty = true;
            }
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
		} else {
            ++hit;
            srand(time(NULL));
            int rndIndex = rand() % (index + 1);
            unsigned long replaceWay = randomIndex[rndIndex];
            randomIndex[rndIndex] = (randomIndex[rndIndex] + 1) % nways;
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++write;
            } else {
                caches[replaceWay][rndIndex].dirty = true;
            }
            ++read;
		}
    } else if (this->replacementAlgo == ReplacementAlgorithm::LEAST_RECENTLY_USED) {
        bool isHit = false;
        bool isInvalid = false;
        unsigned long minWay = 0;
	    ++age;
        for (int way = 0 ; way < nways ; way++) {
            if (!caches[way][index].valid) {
                caches[way][index].tag = tag;
                caches[way][index].valid = true;
                caches[way][index]._age = age;
                isInvalid = true;
                break;
            } else if (caches[way][index].tag == tag && caches[way][index].valid) {
                isHit = true;
                caches[way][index]._age = age;
                break;
           }
        }
        if (!isHit && isInvalid) {
            ++miss;
                if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                    ++write;
                }
        } else if (!isHit) {
            for (int way = 0 ; way < nways ; way++) {
                    if (caches[way][index]._age <  caches[minWay][index]._age) 
                        minWay = way;
            }
            caches[minWay][index]._age = age;
            caches[minWay][index].tag = tag;
            ++miss;
            if (caches[minWay][index].dirty) {
                ++write;
                caches[minWay][index].dirty = false;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++read;
                ++write;
            } else {
                ++read;
                caches[minWay][index].dirty = true;
            }
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
        } else {
            ++hit;
            for (int way = 0 ; way < nways ; way++) {
                    if (caches[way][index]._age <  caches[minWay][index]._age) 
                        minWay = way;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++write;
            } else {
                caches[minWay][index].dirty = true;
            }
            ++read;
        }
    } else if (this->replacementAlgo == ReplacementAlgorithm::LEAST_FREQUENTLY_USED) {
        bool isHit = false;
        bool isInvalid = false;
        unsigned long minWay = 0;
	    ++age;
        for (int way = 0 ; way < nways ; way++) {
            if (!caches[way][index].valid) {
                caches[way][index].tag = tag;
                caches[way][index].valid = true;
                caches[way][index]._age = age;
                isInvalid = true;
                break;
            } else if (caches[way][index].tag == tag && caches[way][index].valid) {
                isHit = true;
                caches[way][index]._age = age;
                break;
           }
        }
        if (!isHit && isInvalid) {
            ++miss;
                if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                    ++write;
                }
        } else if (!isHit) {
            for (int way = 0 ; way < nways ; way++) {
                    if (caches[way][index]._age >  caches[minWay][index]._age) 
                        minWay = way;
            }
            caches[minWay][index]._age = age;
            caches[minWay][index].tag = tag;
            ++miss;
            if (caches[minWay][index].dirty) {
                ++write;
                caches[minWay][index].dirty = false;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++read;
                ++write;
            } else {
                ++read;
                caches[minWay][index].dirty = true;
            }
            if (this->writePolicy == WritePolicy::WRITE_ALLOCATE) {
                ++write;
            }
        } else {
            ++hit;
            for (int way = 0 ; way < nways ; way++) {
                    if (caches[way][index]._age > caches[minWay][index]._age) 
                        minWay = way;
            }
            if (this->writePolicy == WritePolicy::WRITE_THROUGH) {
                ++write;
            } else {
                caches[minWay][index].dirty = true;
            }
            ++read;
        }
    } else {
        fprintf(stderr, "ERROR: Unknown Replacement Algorithm\n\n");
        exit(-1);
    }
	++this->nAccess;
}

void Associativity::tokenizeAddr(unsigned long addr, unsigned long *tag, unsigned long *index, unsigned long *byteOffset) {
    unsigned long tmp;
    *tag = addr >> (this->byteOffsetLength + this->indexLength);
    tmp = 0;
    for (int i = 0 ; i < this->indexLength; i++) {
        tmp <<= 1;
        tmp |= 0x01;
    }
    *index = addr >> (this->byteOffsetLength) & tmp;
    tmp = 0;
    for (int i = 0 ; i < this->byteOffsetLength ; i++) {
        tmp <<= 1;
        tmp |= 0x01;
    }
    *byteOffset = addr & tmp;

}

void Associativity::printStatus() {
	printf("Entry size: %lu entries\n"
		   "Tag length: %lu bit(s)\n"
		   "Index length: %lu bit(s)\n"
		   "Byte offset length: %lu bit(s)\n",
		   caches.size(), tagLength, indexLength, byteOffsetLength);
}
