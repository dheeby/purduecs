
#ifndef command_h
#define command_h

// Command Data Structure
struct SimpleCommand {
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;

	// Number of arguments
	int _numberOfArguments;
	char ** _arguments;
	
    char ** filelist;

    void escapeCharacters(char *);
    void expandTilde(char *);
    void expandEnvar(char *);
    void expandWildcard(char *, char *);
    void sortArray(char **&, int);

	SimpleCommand();
	void insertArgument( char * argument );
};

struct Command {
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand ** _simpleCommands;
	char * _outFile;
	char * _inputFile;
	char * _errFile;
	int _background;
    int _appendOutput;

	void prompt();
	void print();
	void execute();
	void clear();
    
    void expandSpecWord(char *);

	Command();
	void insertSimpleCommand( SimpleCommand * simpleCommand );

	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;
};

#endif
