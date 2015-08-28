#include <stdio.h>

// It prints the bits in bitmap as 0s and 1s.
void printBits(unsigned int bitmap)
{
	int i;
	
	for (i = 31; i >= 0; i--) {
		printf("%d", getBitAt(bitmap, i));
	}
	printf("\n");
	
	for (i = 31; i >= 0; i--) {
		printf("%d", i % 10);
	}
	printf("\n");
}


// Sets the ith bit in *bitmap with the value in bitValue (0 or 1)
void setBitAt( unsigned int *bitmap, int i, int bitValue ) {
	unsigned int addValue = (unsigned int)(bitValue << i);
	if (getBitAt(*bitmap, i) == 0) {
		*bitmap += (bitValue << i);
	} else {
		*bitmap -= (1 << i);
	}
}

// It returns the bit value (0 or 1) at bit i
int getBitAt( unsigned int bitmap, unsigned int i) {
	return (bitmap << 31 - i) >> 31;
}

// It returns the number of bits with a value "bitValue".
// if bitValue is 0, it returns the number of 0s. If bitValue is 1, it returns the number of 1s.
int countBits( unsigned int bitmap, unsigned int bitValue) {
	int i, count = 0;
	for (i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == bitValue) {
			count++;
		}
	}
	return count;

}

// It returns the number of largest consecutive 1s in "bitmap".
// "*position" is set to the beginning of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	int i, pos = 0, currentCount = 0, maxPos = 0, maxOnes = 0;
	
	// Check all bits starting with least significant bit. Increment the count until a zero
	// is encountered. If a zero is found, compare current count of ones to the max count of
	// ones and save the larger. Reset the position and counter.
	for (i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == 1) {
			currentCount++;
		} else {
			if (currentCount > maxOnes) {
				maxOnes = currentCount;
				maxPos = pos;
			}
			currentCount = 0;
			pos = i + 1;
		}
	}

	// Extra check for when most significant bit is 1.
	if (currentCount > maxOnes) {
		maxOnes = currentCount;
		maxPos = pos;
	}

	*position = maxPos;
	return maxOnes;
}


