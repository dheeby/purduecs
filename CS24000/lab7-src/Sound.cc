
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "Sound.h"

// Constructor, empty wave file with default parameters
Sound::Sound(int numChannels, int sampleRate, int bitsPerSample) {
	this->numChannels = numChannels;
	this->sampleRate = sampleRate;
	this->bitsPerSample = bitsPerSample;
}

// Destructor
Sound::~Sound(void) {
	delete this->hdr;
}

// Read a wave file from file name. Samples and parameters are overwritten
bool
Sound::read(const char * fileName) {
	// Open a file in read mode
	FILE * f = fopen(fileName, "r");
	if (f == NULL) {
		printf("Could not open file: %s\n", fileName);
		return false;
	}

	// Get size of file
	struct stat buf;
	stat(fileName, &buf);
	int fsize = buf.st_size;

	// Allocate memory for wave file
	WaveHeader * hdr = (WaveHeader *)malloc(fsize + 1);

	// Read file
	fread(hdr, fsize, 1, f);

	fclose(f);

	// FIll up the header
	this->numChannels = hdr->numChannels[0];
	this->sampleRate = getLittleEndian4(hdr->sampleRate);
	this->bitsPerSample = hdr->bitsPerSample[0] + (hdr->bitsPerSample[1] << 8);
	this->bytesPerSample = (this->bitsPerSample / 8) * this->numChannels;
	this->hdr = hdr;
	this->lastSample = getLittleEndian4(hdr->subchunk2Size) / this->bytesPerSample;
	this->maxSamples = this->lastSample;
	this->fileSize = fsize;
	this->dataSize = fsize - (sizeof(WaveHeader) - 1);
	
	return true;
}

// Write wave file in fileName
void
Sound::write(const char * fileName) {
	FILE * f = fopen(fileName, "w+");
	if (f == NULL) {
		exit(1);
	}

	fwrite(this->hdr, this->fileSize, 1, f);

	fclose(f);
}

// Add a new sample at the end of the wave file
void
Sound::add_sample(int sample) {
	/*
	// Increment variables to account for one extra sample
	lastSample++;
	maxSamples++;
	fileSize += bytesPerSample;
	dataSize += bytesPerSample;
	assignLittleEndian4(hdr->subchunk2Size, this->dataSize);

	// Reallocate header with room for one more sample
	hdr = (WaveHeader *)realloc(hdr, fileSize);
	
	// Add the sample to the end of data
	assignLittleEndian4(&this->hdr->data[lastSample * bytesPerSample], sample);
	*/
	lastSample++;
	assignLittleEndian4(&hdr->data[lastSample * bytesPerSample], sample);
}

// Get the ith sample
int
Sound::get_sample(int i) {
	return getLittleEndian4(&hdr->data[i * bytesPerSample]);
}

// Append a Sound src to the end of this wave file
void
Sound::append(Sound * src) {
	/*
	// Update variables to account for appended data
	this->maxSamples += src->maxSamples;
	this->fileSize += src->dataSize;
	this->dataSize += src->dataSize;

	// Allocate memory in new WaveHeader with enough room for this data plus src data
	WaveHeader * hdr = (WaveHeader *)malloc(this->fileSize + 1);
	
	// Fill up header in hdr
	hdr->chunkID[0]='R'; 
	hdr->chunkID[1]='I'; 
	hdr->chunkID[2]='F'; 
	hdr->chunkID[3]='F';
	assignLittleEndian4(hdr->chunkSize, this->fileSize - 8);
	hdr->format[0]='W'; hdr->format[1]='A'; hdr->format[2]='V'; hdr->format[3]='E';
	hdr->subchunk1ID[0]='f'; hdr->subchunk1ID[1]='m'; hdr->subchunk1ID[2]='t'; hdr->subchunk1ID[3]=' ';
	assignLittleEndian4(hdr->subchunk1Size, 16);
	hdr->audioFormat[0] = 1; hdr->audioFormat[1] = 0;
	hdr->numChannels[0] = this->numChannels; hdr->numChannels[1] = 0;
	assignLittleEndian4(hdr->sampleRate, this->sampleRate);
	assignLittleEndian4(hdr->byteRate, this->numChannels * this->sampleRate * this->bitsPerSample / 8);
	hdr->blockAlign[0] = this->numChannels * this->bitsPerSample / 8; hdr->blockAlign[1] = 0;
	hdr->bitsPerSample[0] = this->bitsPerSample; hdr->bitsPerSample[1] = 0;
	hdr->subchunk2ID[0]='d'; hdr->subchunk2ID[1]='a'; hdr->subchunk2ID[2]='t'; hdr->subchunk2ID[3]='a';
	assignLittleEndian4(hdr->subchunk2Size, this->dataSize);
	
	int i, j = 0;
	
	// Copy data from this data to new hdr
	for (i = 0; i <= this->lastSample * this->bytesPerSample; i++) {
		hdr->data[i] = this->hdr->data[i];
	}

	// Append src data to new hdr data
	for (i = this->lastSample * this->bytesPerSample; i <= this->maxSamples * this->bytesPerSample; i++) {
		hdr->data[i] = src->hdr->data[j++];
	}
	this->lastSample = dataSize / this->bytesPerSample;
	
	// Replace this hdr with new hdr
	this->hdr = hdr;
	*/
	fileSize += src->dataSize;
	dataSize += src->dataSize;
	maxSamples += src->maxSamples;

	hdr = (WaveHeader *)realloc(hdr, fileSize);
	assignLittleEndian4(hdr->subchunk2Size, dataSize);

	
	for (int i = 0; i <= src->maxSamples; i++) {
		add_sample(src->get_sample(i));
	}
	
	
}

// Get int value from number in little endian
unsigned int
Sound::getLittleEndian4(unsigned char * value) {
	unsigned int num = 0;
	num += value[0];
	num += value[1] << 8;
	num += value[2] << 16;
	num += value[3] << 24;

	return num;
}

// Assign a value as little endian
void
Sound::assignLittleEndian4(unsigned char * dest, unsigned int value) {
	dest[0] = value & 0xFF;
	dest[1] = (value >> 8) & 0xFF;
	dest[2] = (value >> 16) & 0xFF;
	dest[3] = (value >> 24) & 0xFF;
}

