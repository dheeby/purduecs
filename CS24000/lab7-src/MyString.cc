
// String Implementation
// IMPORTANT: Do not use any of the functions in the string C runtime library
// Example. Do not use strcpy, strcmp, etc. Implement your own

// IMPORTANT: See the MyString.h file for a description of
// what each method needs to do.

#include <stdio.h>
#include <stdlib.h>
#include "MyString.h"

// My own implementation of strlen
int
MyString::slength(const char *s) const
{
  // Add implementation here
  int count = 0;
  while (*s != '\0') {
	s++;
	count++;
  }
  return count;
}

// Initialize _s. Allocate memory for _s and copy s into _s
void
MyString::initialize(const char * s)
{
  	// Add implementation here
  	// Allocate memory for _s.
	_s = new char[slength(s)];
  	// Copy s into _s
  	int i;
	for (i = 0; i < slength(s); i++) {
		_s[i] = s[i];
	}
	_s[i] = '\0';
}

// Create a MyString from a C string
MyString::MyString(const char * s)
{
  initialize(s);
}

// Create a MyString from a copy of another string
MyString::MyString(const MyString &s)
{
  initialize(s._s);
}

// Create a MyString with an empty string
MyString::MyString()
{
  _s = new char[1];
  *_s = 0;
}

// Assignment operator. Without this operator the assignment is
// just a shallow copy that will copy the pointer _s. If the original _s
// goes away then the assigned _s will be a dangling reference.
MyString &
MyString::operator = (const MyString & other) {
  if (this != &other) // protect against invalid self-assignment
  {
    // deallocate old memory
    delete [] _s;

    // Initialize _s with the "other" object.
    initialize(other._s);

    // by convention, always return *this
    return *this;
  }
}

// Obtain a substring of at most n chars starting at location i
// if i is larger than the length of the string return an empty string.
MyString
MyString::substring(int i, int n)
{
	// Add implementation here
	// Make sure that i is not beyond the end of string
	// Allocate memory for substring
	// Copy characters of substring
	MyString sub;

	if (i > slength(_s)) {
		return sub;
	}

	int subSize = n;
	if ((i + n) > slength(_s)) {
		subSize = slength(_s) - 1;
	}

	sub._s = new char[subSize + 1];

	for (int j = 0; j < subSize; j++) {
		sub._s[j] = _s[i + j];
	}

	sub._s[subSize] = '\0';


  	// Return substring
  	return sub;
}

// Remove at most n chars starting at location i
void
MyString::remove(int i, int n)
{
	// Add implementation here
	// If i is beyond the end of string return
	// If i+n is beyond the end trunc string
	// Remove characters
	if (i > slength(_s)) {
		return;
	}
	if ((i + n) > slength(_s)) {
		char * s = new char[i + 1];
		for (int j = 0; j <= i; j++) {
			s[j] = _s[j];
		}
		s[i + 1] = '\0';
		_s = s;
	} else {
		char * s = new char[slength(_s) - n];
		int j = 0;
		for (int k = 0; k < i; k++) {
			s[k] = _s[k];
			j++;
		}
		for (int k = i + n; k < slength(_s); k++) {
			s[j] = _s[k];
			j++;
		}
		s[j] = '\0';
		_s = s;
	}
}

// Return true if strings "this" and s are equal
bool
MyString::operator == (const MyString & s)
{
	if (slength(s._s) != slength(_s)) {
		return false;
	}
	// Add implementation here
	for (int i = 0; i < slength(_s); i++) {
		if (s._s[i] != _s[i]) {
			return false;
		}
	}
	return true;
}


// Return true if strings "this" and s are not equal
bool
MyString::operator != (const MyString & s)
{
	return (!(*this == s._s));
}

// Return true if string "this" and s is less or equal
bool
MyString::operator <= (const MyString & s)
{
	int len1 = slength(_s);
	int len2 = slength(s._s);
	int len;
	if (len1 > len2) {
		len = len1;
	} else {
		len = len2;
	}
	for (int i = 0; i < len; i++) {
		if (_s[i] > s._s[i]) {
			return false;
		}
		if (_s[i] < s._s[i]) {
			return true;
		}
		continue;
	}
	return true;
}

// Return true if string "this" is greater than s
bool
MyString::operator > (const MyString & s)
{
	return (!(*this <= s._s));
}

// Return character at position i.  Return '\0' if out of bounds.
char
MyString::operator [] (int i)
{
  // Add implementation here
  if (i >= slength(_s)) {
	return '\0';
  }
  return _s[i];
}

// Return C representation of string
const char *
MyString::cStr()
{
  // Add implementation here
  return _s;
}

// Get string length of this string.
int
MyString::length() const
{
  // Add implementation here
  return slength(_s);
}

// Destructor. Deallocate the space used by _s
MyString::~MyString()
{
  // Add implementation here
  delete [] _s;
}

// Concatanate two strings (non member method)
MyString operator + (const MyString & s1, const MyString & s2)
{
	// Add implementation here
	// Allocate memory for the concatenated string
	// Add s1
	// Add s2
	int len = s1.length() + s2.length();
	MyString s;
	s._s = new char[len];
	
	for (int i = 0; i < s1.length(); i++) {
		s._s[i] = s1._s[i];
	}
	for (int i = 0; i < s2.length(); i++) {
		s._s[s1.length() + i] = s2._s[i];
	}

	s._s[len] = '\0';

	return s;
}

