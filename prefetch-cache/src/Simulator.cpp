/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Simulator.cpp
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

int hitCount = 0;
int missCount = 0;
int memReadCount = 0;
int lrcTimeStamp = 0;
unsigned int setBits = 0;
unsigned int blockBits = 0;
unsigned int tagBits = 0;
unsigned char label[500];
int addressCount = 0;

typedef struct {
	int lineValidity;
	unsigned long long lineTag;
	int lruCount;
} line_st;

// Set structure has a pointer to all the line within the set.
typedef struct {
	line_st* pLines;
} set_st;

// Cache structure has a pointer to all the sets with the cache
typedef struct {
	set_st* pSets;
} cache_st;


void printResults(bool bPrefetch, char** argv) {
	int space = 20;
	printf("\n=========================\n");
	if (bPrefetch) {
		printf("%*s", space, "With Prefetch\n");
	}
	printf("=========================\n");
	printf("\nAddress file: %s (%s)\n\n", label, argv[5]);
	printf("Read: %d\n\n", memReadCount);
	printf("Hit: %d\n", hitCount);
	printf("Miss: %d\n", missCount);
	printf("Miss Rate: %.4f\n", (float)missCount/(float)addressCount);
}

void trimLeadingAndTrailingSpaces(char *s) {
	int  i, j;
	for (i = 0; s[i] == ' ' || s[i] == '\t'; i++);	
	for (j = 0; s[i]; i++) {
		s[j++] = s[i];
	}
	s[j] = '\0';
	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] != ' ' && s[i] != '\t')
			j = i;
	}
	s[j+1] = '\0';
}

unsigned int myLog2(unsigned int x) {
	unsigned int ans = 0;
	while (x >>= 1) ans++;
	return ans;
}

bool powerOfTwo(int n) {
	if (n==0) {
		return false;
	}

   return (ceil(log2(n)) == floor(log2(n)));
}

void splitAddress(unsigned long long address, unsigned long long* pTag, unsigned int* pSetIndex, unsigned int* pBlockOffset) {
	unsigned long long tempValue = 0;
	*pTag = address >> (setBits + blockBits);
	tempValue = address << tagBits;
	*pSetIndex = (unsigned int)(tempValue >> (tagBits + blockBits));
	tempValue = address << (tagBits + setBits);
	*pBlockOffset = (unsigned int)(tempValue >> (tagBits + setBits));
}

int getHitLine(set_st * pSet, int numLines, unsigned long long tag) {
	for (int line = 0; line < numLines; line++) {
		// Make sure the line is valid data and the tag is matching
		if ((pSet->pLines[line].lineValidity == 1) &&
			(pSet->pLines[line].lineTag == tag)) {
			return line;
		}
	}
	return -1;
}

int getEmptyLine(set_st * pSet, int numLines) {
	for (int line = 0; line < numLines; line++) {
		// Find an empy line
		if (pSet->pLines[line].lineValidity == 0) {
			return line;
		}
	}
	return -1;
}

int getEvictionLine(set_st * pSet, int numLines) {
	int minLruCount = INT_MAX;
	int lruLine = -1;
	for (int line = 0; line < numLines; line++) {
		// Find block to evict
		if (pSet->pLines[line].lruCount < minLruCount) {
			minLruCount = pSet->pLines[line].lruCount;
			lruLine = line;
		}
	}
	return lruLine;
}

void processMissedCacheOperation(set_st* pSet, int numLines, unsigned long long tag) {
	// check for empty line
	int line = getEmptyLine(pSet, numLines);
	if (line > -1) {
		// Found a empy line read memory
		pSet->pLines[line].lineValidity = 1;
		pSet->pLines[line].lineTag = tag;
		pSet->pLines[line].lruCount = lrcTimeStamp;
		lrcTimeStamp++;

	}
	else {
		// find block to evict with prefetched block. 
		line = getEvictionLine(pSet, numLines);
		if (line > -1) {
			// Found a block to evict
			pSet->pLines[line].lineValidity = 1;
			pSet->pLines[line].lineTag = tag;
			pSet->pLines[line].lruCount = lrcTimeStamp;
			lrcTimeStamp++;
		}
		else {
			fprintf(stderr, "ERROR: Did not find a block evict\n");
			exit(-1);
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 6) {
		fprintf(stderr, "ERROR: Incorrect input parameters.\n\n");
		exit(-1);
	}
	if (!powerOfTwo(atoi(argv[1]))) {
		fprintf(stderr, "ERROR: Cache size should be a power of 2.\n\n");
		exit(-1);
	}
	if (!powerOfTwo(atoi(argv[4]))) {
		fprintf(stderr, "ERROR: Block size should be a power of 2.\n\n");
		exit(-1);
	}
	if (atoi(argv[4]) > atoi(argv[1])) {
		fprintf(stderr, "ERROR: Block size is greater than Cache size\n\n");
        exit(-1);
	}
	unsigned int cachesize = atoi(argv[1]);
	unsigned int blocksize = atoi(argv[4]);
	unsigned int numSets = cachesize / blocksize;
	unsigned int numLines = 1;
	if (memcmp(argv[2], "assoc:", 6) == 0) {
		char* pstr = strchr(argv[2], ':');
		if (pstr) {
			numLines = atoi(pstr + 1);
			if (!powerOfTwo(numLines)) {
				fprintf(stderr, "ERROR: N way should be a power of 2.\n\n");
				exit(-1);
			}
		}
	}
	numSets = numSets / numLines;
	setBits = myLog2(numSets);
	blockBits = myLog2(blocksize);
	tagBits = 64 - (setBits + blockBits);

	cache_st cache;
	// Memory alloction for all sets in the cache
	cache.pSets = (set_st*)malloc(numSets * sizeof(set_st));
	for (unsigned int i = 0; i < numSets; i++) {
		memset(&cache.pSets[i], 0, sizeof(set_st));
	}

	// Memory alloction for all lines in all sets
	for (unsigned int i = 0; i < numSets; i++) {
		cache.pSets[i].pLines = (line_st*)malloc(numLines * sizeof(line_st));
		for (unsigned int j = 0; j < numLines; j++) {
			memset(&cache.pSets[i].pLines[j], 0, sizeof(set_st));
		}
	}

	unsigned long long address;
	char zBuffer[512];
	bool bPrefetch = true;
	unsigned long long addressTag = 0;
	unsigned int setIndex = 0;
	unsigned int blockOffset = 0;
	cache.pSets = (set_st*)malloc(numSets * sizeof(set_st));
	for (unsigned int i = 0; i < numSets; i++) {
		memset(&cache.pSets[i], 0, sizeof(set_st));
	}

	// Memory alloction for all lines in all sets
	for (unsigned int i = 0; i < numSets; i++) {
		cache.pSets[i].pLines = (line_st*)malloc(numLines * sizeof(line_st));
		for (unsigned int j = 0; j < numLines; j++) {
			memset(&cache.pSets[i].pLines[j], 0, sizeof(set_st));
		}
	}
	// Memory alloction for all lines in all sets
	FILE* fd = fopen(argv[5], "r");
	bool isSkipLabel = false;
	if (fd) {
		while (fgets(zBuffer, 255, fd) != NULL) {
			trimLeadingAndTrailingSpaces(zBuffer);
			if (memcmp(zBuffer, "#eof", 4) != 0) {
				if(!isSkipLabel) {
					sscanf(zBuffer, "%s", label);
					isSkipLabel = true;
					continue;
				}
				sscanf(zBuffer, "0x%llx", &address);
				addressCount++;
				splitAddress(address, &addressTag, &setIndex, &blockOffset);
				set_st set = cache.pSets[setIndex];
				// check for hit
				int line = getHitLine(&set, numLines, addressTag);
				if (line > -1) {
					// Hit
					hitCount++;
					set.pLines[line].lruCount = lrcTimeStamp;
					lrcTimeStamp++;
				} else {
					// Miss
					missCount++;
					// Both read and writing require to reading the memory
					memReadCount++;
					processMissedCacheOperation(&set, numLines, addressTag);
					
					if (bPrefetch) {
						// No miss for prefetch as cpu did not request
						splitAddress(address + blocksize, &addressTag, &setIndex, &blockOffset);
						set = cache.pSets[setIndex];
						int line = getHitLine(&set, numLines, addressTag);
						if (line > -1) {
							// Hit
							hitCount++;
						}
						else {
							memReadCount++;
							processMissedCacheOperation(&set, numLines, addressTag);
						}
					}
				}
			}
			else {
				break;
			}
		}
		fclose(fd);
	} else {
		fprintf(stderr, "ERROR: File not found.\n\n");
		exit(-1);
	}
	printResults(bPrefetch, argv);
	// Deallocate memory for all the lines in all the sets
	for (unsigned int i = 0; i < numSets; i++)
	{
		if (cache.pSets[i].pLines) {
			free(cache.pSets[i].pLines);
		}
	}
	// Deallocation memory for the sets.
	if (cache.pSets) {
		free(cache.pSets);
	}
	return 0;
}
