#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 4;
	static const long BUILD = 11;
	static const long REVISION = 49;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 793;
	#define RC_FILEVERSION 2,4,11,49
	#define RC_FILEVERSION_STRING "2, 4, 11, 49\0"
	static const char FULLVERSION_STRING[] = "2.4.11.49";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 5;
	

#endif //VERSION_H
