/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Simulator.cpp
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "Utility.h"
#include "Cache.h"
#include "DirectMapped.h"
#include "Associativity.h"

void trimLeadingAndTrailingSpaces(char *s); 

int main(int argc, char *argv[]) {
	Utility utility;
	utility.tokenizeArgv(argc, argv);

	if(!utility.isSetFileName) {
		fprintf(stderr, "ERROR: Please specify address file\n\n");
		// Show usage
		utility.showUsage(argv[0]);
		exit(-1);
	}

	FILE *fp;
	int read, write;
	char buff[1025];
	unsigned long addrTmp;
	char label[1025];

	if(!(fp = fopen(utility.fileName, "r"))) {
		perror(utility.fileName);
		exit(-1);
	}

	Cache *cache;
	if(utility.cacheType == CacheType::DIRECT_MAPPED) {
		cache = new DirectMapped(utility.cacheSize, utility.blockSize, utility.writePolicy);
	} else {
		cache = new Associativity(utility.nWay, utility.replacementAlgorithm, utility.writePolicy, utility.cacheSize);
	}

	// Read file
	bool isSkipLabel = false;
	while (fgets(&buff[0], 1024, fp)) {
		trimLeadingAndTrailingSpaces(buff);
		if(!isSkipLabel) {
			sscanf(buff, "%s", label);
			isSkipLabel = true;
			continue;
		}
		sscanf(buff, "0x%lx", &addrTmp);
		cache->access(addrTmp);
	}

	printf("\n=====================\n");
	printf(" Cache Specification \n");
	printf("=====================\n");

	if(utility.cacheType == CacheType::DIRECT_MAPPED) {
		printf("Cache type: Direct mapped\n"
			   "Cache size: %lu bytes\n"
			   "Block size: %lu bytes\n\n",
			   utility.cacheSize,
			   utility.blockSize);
	} else {
		printf("Cache type: Associativity\n"
			   "Cache size: %lu bytes\n"
			   "N-way: %u bytes\n"
			   "Replacement algorithm: %s\n"
			   "Write policy: %s\n\n",
			   utility.cacheSize,
			   utility.nWay,
		   	   utility.replacementAlgorithm == ReplacementAlgorithm::CLOCK ?  "Clock" :
				  ReplacementAlgorithm::RANDOM ? "Random" : 
				  ReplacementAlgorithm::LEAST_RECENTLY_USED ? "Least recently used" : "Least frequently used",
			   utility.writePolicy == WritePolicy::WRITE_THROUGH ? "Write through" : "Write allocate");
	}

	cache->printStatus();

	printf("\nAddress file: %s (%s)\n", label, utility.fileName);

	printf("\nRead: %ld\n"
		   "Write: %ld\n\n" 
		   "Hit: %ld\n"
		   "Miss: %ld\n"
		   "Miss Rate: %.4f\n", cache->getRead(), cache->getWrite(), cache->getHit(), cache->getMiss(),(float)cache->getMiss() / (float)cache->getNAccess());
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
