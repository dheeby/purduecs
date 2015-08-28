
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Convert freq to wave file. The freq file has a sequence of frequency and duration pairs that
// make the song. You need to generate the corresponding tones for that frequency and duration and
// append them to the wave file. See the 
// Study the file simple-gen.c that is used to generate a single frequency
// sound for a certain duration.

typedef struct WaveHeader {
	unsigned char chunkID[4];	// big endian
	unsigned char chunkSize[4];	// little endian
	unsigned char format[4];	// big endian
	unsigned char subchunk1ID[4];	// big endian
	unsigned char subchunk1Size[4];	// little endian
	unsigned char audioFormat[2];	// little endian
	unsigned char numChannels[2];	// little endian
	unsigned char sampleRate[4];	// little endian
	unsigned char byteRate[4];	// little endian
	unsigned char blockAlign[2];	// little endian
	unsigned char bitsPerSample[2];	// little endian
	unsigned char subchunk2ID[4];	// big endian
	unsigned char subchunk2Size[4];	// little endian
	unsigned char data[1];		// little endian
} WaveHeader;

void assignLittleEndian4(unsigned char * dest, unsigned int value) {
	dest[0] = value & 0xFF;
	dest[1] = (value >> 8) & 0xFF;
	dest[2] = (value >> 16) & 0xFF;
	dest[3] = (value >> 24) & 0xFF;
}

int main(int argc, char ** argv)
{
        if (argc <2) {
                printf("freq2wav freq-file\n");
                exit(1);
        }
	
	char * filename = argv[1];
	if (!strstr(filename, ".freq")) {
		printf("Expected .freq file.\n");
		exit(1);
	}
	
	int i, len = strlen(filename);
	char * prename = (char *)malloc(sizeof(char) * len);
	for (i = 0; i < len - 5; i++) {
		prename[i] = filename[i];
	}
	prename[i] = '\0';
	
	printf("freqFile: %s\n", filename);
	printf("waveFile: %s.wav\n", prename);

	FILE * f = fopen(filename, "r");
	if (f == NULL) {
		printf("File \"%s\" not found\n", filename);
		exit(1);
	}

	//
	// 1. Read freq-file. The freq file contains pairs (frequency, msecs)
	//    that indicates the signal frequency and duration that will be
	//    added to the wave file.
	// 2. For every freq.msecs pair generate the tone and added it to the wave file.
	// 3. Store the wave file. If the input file was file.freq, then store the wav file
	//    as file.wav
	int * freq = (int *)malloc(sizeof(int) * 100);
	int * msec = (int *)malloc(sizeof(int) * 100);
	int * freqP = freq;
	int * msecP = msec;
	int totalRead = 0;
	int msecs = 0;

	printf("Frequency file: (Hz,ms)");

	while (fscanf(f, "%d %d", freqP, msecP) > 0) {
		if ((totalRead % 10) == 0) {
			printf("\n%d: ", totalRead);
		}
		printf("(%d,%d) ", freq[totalRead], msec[totalRead]);
		msecs += *msecP;
		totalRead++;
		freqP++;
		msecP++;
	}
	printf("\n");
	freqP = freq;
	msecP = msec;

	unsigned int numChannels = 1;
	unsigned int sampleRate = 44100;
	unsigned int bitsPerSample = 16;
	unsigned int numSamples = (msecs * sampleRate) / 1000;
	unsigned int dataSize = numSamples * numChannels * (bitsPerSample / 8);
	unsigned int fileSize = sizeof(WaveHeader) - 1 + dataSize;

	WaveHeader * hdr = (WaveHeader *)malloc(fileSize);

	hdr->chunkID[0] = 'R'; hdr->chunkID[1] = 'I'; hdr->chunkID[2] = 'F'; hdr->chunkID[3] = 'F';
	assignLittleEndian4(hdr->chunkSize, fileSize - 8);
	hdr->format[0] = 'W'; hdr->format[1] = 'A'; hdr->format[2] = 'V'; hdr->format[3] = 'E';
	hdr->subchunk1ID[0] = 'f'; hdr->subchunk1ID[1] = 'm'; hdr->subchunk1ID[2] = 't'; hdr->subchunk1ID[3] = ' ';
	assignLittleEndian4(hdr->subchunk1Size, 16);
	hdr->audioFormat[0] = 1; hdr->audioFormat[1] = 0;
	hdr->numChannels[0] = numChannels; hdr->numChannels[1] = 0;
	assignLittleEndian4(hdr->sampleRate, sampleRate);
	assignLittleEndian4(hdr->byteRate, numChannels * sampleRate * bitsPerSample / 8);
	hdr->blockAlign[0] = numChannels * bitsPerSample / 8; hdr->blockAlign[1] = 0;
	hdr->bitsPerSample[0] = bitsPerSample; hdr->bitsPerSample[1] = 0;
	hdr->subchunk2ID[0] = 'd'; hdr->subchunk2ID[1] = 'a'; hdr->subchunk2ID[2] = 't'; hdr->subchunk2ID[3] = 'a';
	assignLittleEndian4(hdr->subchunk2Size, dataSize);

	int littleEndian = 0;
	int test = 5;
	if (*(char *)&test == 5) {
		littleEndian = 1;
	}
	
	int m, n, p;
	i = 0;

	printf("Number of Samples: %d\n", numSamples);

	//printf("FILLING DATA....\n");
	
	/*
	for (m = 0; m < totalRead; m++) {
		for (n = 0; n < msec[m]; n++) {
			for (p = 0; p <= 44; p++) {
				short value = 32767 * sin(3.1415 * freq[m] * i / sampleRate);
				if (littleEndian) {
					hdr->data[2 * i] = *((unsigned char *)&value);
					hdr->data[2 * i + 1] = *((unsigned char *)&value + 1);
				} else {
						hdr->data[2 * i + 1] = *((unsigned char *)&value);
					hdr->data[2 * i] = *((unsigned char *)&value + 1);
				}
				i++;
			}
		
	}
	*/

	
	//printf("\n**Total Time: %d, Samples: %d **", msecs, i);
	/*
	for (i = 0; i < numSamples; i++) {
		short value = 32767 * sin(3.1415 * freq[i] *  i / sampleRate);
		if (littleEndian) {
			hdr->data[2 * i] = *((unsigned char *)&value);
			hdr->data[2 * i + 1] = *((unsigned char *)&value + 1);
		} else {
			hdr->data[2 * i + 1] = *((unsigned char *)&value);
			hdr->data[2 * i] = *((unsigned char *)&value + 1);
		}
	}
	*/
	
	// WRITE TO DISK
	//fclose(f);

	char * saveName = (char *)malloc(sizeof(char) * strlen(prename) + 4);
	
	saveName = strcat(prename, ".wav\0");

	//printf("*** FILENAME ***: %s\n", saveName);

	f = fopen(saveName, "w+");
	if (f == NULL) {
		printf("Could not create file\n");
		perror("fopen");
		exit(1);
	}

	fwrite(hdr, fileSize, 1, f);

	fclose(f);
}
