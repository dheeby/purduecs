#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Sound.h"
#include "MyString.h"

bool isAlpha(int c) {
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
		return true;
	}
	return false;
}

char toLower(int c) {
	if ((c >= 'A') && (c <= 'Z')) {
		return c + 32;
	}
	return c;
}

int main(int argc, char ** argv) {
	if (argc == 1) {
		printf("Usage: TextToSpeech <phrase>\n");
		exit(1);
	}
	char * phrase = argv[1];
	
	char * buf = (char *)malloc(32);
	MyString phraseWords[50];

	int j = 0;
	int numWords = 0;

	for (int i = 0; i <= strlen(phrase); i++) {
		if (isAlpha(phrase[i])) {
			buf[j++] = toLower(phrase[i]);
		} else {
			if ((strlen(buf) > 0)) {
				if (phrase[i] == '\0' || phrase[i] == ' ') {
					buf[j] = '\0';
					MyString s;
					s = s + "words/" + strcat(buf, ".wav");
					
					phraseWords[numWords++] = s;
					
					j = 0;
					free(buf);
					buf = (char *)malloc(32);
				}
			}
		}
	}
	
	Sound * sounds[numWords];
	sounds[0] = new Sound();

	Sound finalsound;
	finalsound.read("words/pause.wav");
	Sound pause;
	pause.read("words/pause.wav");
	
	int k = 1;

	for (int i = 0; i < numWords; i++) {
		sounds[k] = new Sound();
		if (sounds[k]->read(phraseWords[i].cStr())) {
			finalsound.append(sounds[k]);
			finalsound.append(&pause);
			k++;
		}
	}
	finalsound.write("phrase.wav");
}
