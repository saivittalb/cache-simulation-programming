/**
 * EE 318 - Computer Architecture and Design
 * Assignment 2 - Cache Simulator Programming 
 *
 * @code Cache.cpp
 * @author Sai Vittal B
 * @copyright 2021
 * MIT License
 */

#include "Cache.h"

Cache::Cache() {
	this->hit = 0;
	this->miss = 0;
	this->read = 0;
	this->write = 0;
	this->nAccess = 0;
	this->wordSize = 4;
}

long Cache::getHit() const {
	return this->hit;
}

long Cache::getMiss() const {
	return this->miss;
}

long Cache::getRead() const {
	return this->read;
}

long Cache::getWrite() const {
	return this->write;
}

long Cache::getNAccess() const {
	return this->nAccess;
}
