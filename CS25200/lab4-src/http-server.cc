
const char *usage =
    "http-server [-f|-t|-p] [<port>]\n"
    "   -f: forks a child process for each new request\n"
    "   -t: creates a new thread for each new request\n"
    "   -p: uses a pool of threads to handle new requests\n"
    "   <port>: port number between 1024 and 65536\n"
    "   **If no concurrency flag is specified, requests are handled\n"
    "       with a single threaded queue\n\n"
    "In a web browser, type <host>:<port><page>\n"
    "   <host>: the host this server is running on\n"
    "   <post>: the port this server is running on\n"
    "   <page>: the page you would like to access or empty for default page\n\n";

const char *serverType = "CS 252 lab4";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

int QueueLength = 5;

// Processes time request
void processTimeRequest(int socket);
void handleConnectionsNonConcurrent(int masterSocket);
void handleConnectionsProcesses(int masterSocket);
void handleConnectionsThreads(int masterSocket);
void handleConnectionsThreadPool(long masterSocket);
void dispatchHTTP(long socket);
void loopthread(long masterSocket);
pthread_mutex_t lock;
pthread_mutex_t m1;

void zombie_handler(int sig) {
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
}

int main(int argc, char **argv) {
    // Print usage if there aren't 2 or 3 arguments
    if (!(argc == 2 || argc == 3)) {
        printf("%d\n", argc);
        fprintf(stderr, "%s", usage);
        exit(-1);
    }

    int port;
    int concurrencyMode;
    if (argc == 3) {
        // Check flags for concurrency mode
        if (!strcmp(argv[1], "-f")) {
            // Create a new process for each request
            printf("Concurrency: processes\n");
            concurrencyMode = 1;
        } else if (!strcmp(argv[1], "-t")) {
            // Create a new thread for each request
            printf("Concurrency: threads\n");
            concurrencyMode = 2;
        } else if (!strcmp(argv[1], "-p")) {
            // Pool of threads
            printf("Concurrency: pool of threads\n");
            concurrencyMode = 3;
        } else {
            // Invalid command
            fprintf(stderr, "%s", usage);
            exit(-1);
        }
        port = atoi(argv[2]);
    } else {
        // No concurrency
        port = atoi(argv[1]);
        concurrencyMode = 0;
    }

    // Set signal to handle zombie processes
    signal(SIGCHLD, zombie_handler);

    // Set the IP address and port for this server
    struct sockaddr_in serverIPAddress;
    memset(&serverIPAddress, 0, sizeof(serverIPAddress));
    serverIPAddress.sin_family = AF_INET;
    serverIPAddress.sin_addr.s_addr = INADDR_ANY;
    serverIPAddress.sin_port = htons((u_short)port);

    // Allocate a socket
    int masterSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (masterSocket < 0) {
        perror("socket");
        exit(-1);
    }

    // Set socket options to reuse port. Otherwise we will
    // have to wait about 2 minutes before reusing the sae port number
    int optval = 1;
    int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR,
	    (char *)&optval, sizeof(int));

    // Bind the socket to the IP address and port
    int error = bind(masterSocket,
	    (struct sockaddr *)&serverIPAddress,
        sizeof(serverIPAddress));
    if (error) {
        perror("bind");
        exit(-1);
    }

    // Put socket in listening mode and set the 
    // size of the queue of unprocessed connections
    error = listen(masterSocket, QueueLength);
    if (error) {
        perror("listen");
        exit(-1);
    }
    
    // Call correct currency mode function
    if (concurrencyMode == 0) {
        handleConnectionsNonConcurrent(masterSocket);
    } else if (concurrencyMode == 1) {
        handleConnectionsProcesses(masterSocket);
    } else if (concurrencyMode == 2) {
        handleConnectionsThreads(masterSocket);
    } else if (concurrencyMode == 3) {
        handleConnectionsThreadPool(masterSocket);
    }
}

void handleConnectionsProcesses(int masterSocket) {
    while (1) {
        // Setup socket to listen to request on
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);

        int slaveSocket = accept(masterSocket,
            (struct sockaddr *)&clientIPAddress,
            (socklen_t *)&alen);

        if (slaveSocket < 0) {
            perror("accept");
            exit(-1);
        }
        
        // Fork a new process for each new request
        int pid = fork();
        if (pid == 0) {
            processTimeRequest(slaveSocket);
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(-1);
        }

        // Close socket in the parent process
        close(slaveSocket);
    }
}

void handleConnectionsThreads(int masterSocket) {
    while (1) {
        // Setup socket to listen to request on
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);

        pthread_mutex_lock(&lock);
        long slaveSocket = accept(masterSocket,
            (struct sockaddr *)&clientIPAddress,
            (socklen_t *)&alen);
        pthread_mutex_unlock(&lock);

        if (slaveSocket < 0) {
            perror("accept");
            exit(-1);
        }
        
        // Create a thread for each new request
        // Set detach state so it does not wait to be joined
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&thread, &attr, (void * (*)(void *)) dispatchHTTP,
            (void *) slaveSocket);
    }
}

void handleConnectionsThreadPool(long masterSocket) {
    // Create a pool of 4 threads, each running the loopthread function
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, (void * (*)(void *))loopthread,
            (void *) masterSocket);
    }
    // Make use of main thread to also handle requests
    loopthread(masterSocket);
}

void loopthread(long masterSocket) {
    while (1) {
        // Setup socket to listen to request on
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);

        pthread_mutex_lock(&lock);
        int slaveSocket = accept(masterSocket,
            (struct sockaddr *)&clientIPAddress,
            (socklen_t *)&alen);
        pthread_mutex_unlock(&lock);

        if (slaveSocket < 0) {
            perror("accept");
            exit(-1);
        }
        
        // Handles the request and closes the socket
        dispatchHTTP(slaveSocket);
    }
}

void handleConnectionsNonConcurrent(int masterSocket) {
    while (1) {
        // Setup socket to listen to request on
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);
        int slaveSocket = accept(masterSocket,
            (struct sockaddr *)&clientIPAddress,
            (socklen_t *)&alen);

        if (slaveSocket < 0) {
            perror("accept");
            exit(-1);
        }
        
        // Process the request
        processTimeRequest(slaveSocket);
        
        // Close the socket
        close(slaveSocket);
    }
}

void dispatchHTTP(long fd) {
    processTimeRequest(fd);
    close(fd);
}

void processTimeRequest(int fd) {
    int n;
    unsigned char newChar;
    unsigned char lastChar = 0;

    char *header = (char *)malloc(sizeof(char) * 1024);
    char *docPath = (char *)malloc(sizeof(char) * 1024);
    int headerLen = 0;
    int hasGET = 0;
    int seenDocPath = 0;
    
    // Read the header from the client
    while (n = read(fd, &newChar, sizeof(newChar))) {
        headerLen++;
        printf("%c", (newChar >= 32 && newChar < 127) ? newChar : ' ');
        
        // Make sure client makes a GET request
        // Parse the document he client is requesting
        // Stop parsing the header when the first <cr><lf> is read
        if (newChar == ' ') {
	        if (!hasGET) {
	            hasGET = 1;
	        } else if (!seenDocPath) {
	            header[headerLen - 1] = 0;
	            strcpy(docPath, header + 4);
	            seenDocPath = 1;
	        }
	    } else if (lastChar == '\015' && newChar == '\012') {
	        break;
	    } else {
	        lastChar = newChar;
	        header[headerLen - 1] = newChar;
	    }
    }

    printf("\n");

    free(header);
    
    int maxcap = 1024;
    char *buf = (char *)malloc(sizeof(char) * maxcap);
    int numRead = 0;
    
    // Read the rest of the header from the client
    while ((n = read(fd, &newChar, sizeof(newChar)))) {
        buf[numRead++] = newChar; 
        // Resize buffer if needed
        // Read until two consecutive <cr><lf> are read
        if (numRead >= maxcap) {
	        maxcap *= 2;
	        buf = (char *) realloc (buf, maxcap);
	    }
        if (lastChar == '\015' && newChar == '\012') {
	        if (numRead > 3) {
	            if (buf[numRead - 3] == '\012' && buf[numRead - 4] == '\015') {
		            break;
		        }
	        }
	    }
    }

    free(buf);

    // Store current working directory
    char *cwd = (char *)malloc(sizeof(char) * 256);
    cwd = getcwd (cwd, 256);

    char *filepath = (char *)malloc(sizeof(char) * 1024);
    strcpy(filepath, cwd);
    
    // Checks for 4 special cases and a default case
    //  1: Requested document is in the root directory
    //  2: Requested document is in the icons directory
    //  3: Requested document is in the htdocs directory
    //  4: Requested document is a cgi script in cgi-bin directory
    //  Default: Check htdocs directory
    if (strlen(docPath) == 1 && *docPath == '/') {
        strcat(filepath, "/http-root-dir/htdocs/index.html");
    } else if (strstr(docPath, "/icons") == docPath) {
        strcat(filepath, "/http-root-dir");
        strcat(filepath, docPath);
    } else if (strstr(docPath, "/htdocs") == docPath) {
        strcat(filepath, "/http-root-dir");
        strcat(filepath, docPath);
    } else if (strstr(docPath, "/cgi-bin") == docPath) {
        strcat(filepath, "/http-root-dir");
        strcat(filepath, docPath);
        
        // Make a pointer to query params if they are specified
        char * questionPtr = strchr(docPath, '?');
        int hasQueryParams = 0;
        if (questionPtr != NULL) {
            questionPtr++;
            hasQueryParams = 1;
        }

        // Fork a child process to run the script
        int tmpout = dup(1);
        int pid;
        if ((pid = fork()) == 0) {
            // Set the REQUEST_METHOD envar
            char reqMethodEnvar[64] = "REQUEST_METHOD=GET\0";
            putenv(reqMethodEnvar);
            
            if (hasQueryParams) {
                int numArgs = 0;
                int maxargs = 32;
                char * arguments[maxargs];
                char * tmp = strdup(questionPtr);
                char * argTokPtr;
                char * plusPtr;

                // Replace all '+' with spaces
                while ((plusPtr = strchr(tmp, '+')) != NULL) {
                    *plusPtr = ' ';
                }

                // Set the QUERY_STRING envar to the query params string
                char queryStringPrefix[512] = "QUERY_STRING=\0";
                strcat(queryStringPrefix, questionPtr);
                putenv(queryStringPrefix);

                //Split the query params, separated by '&'
                while ((argTokPtr = strtok(tmp, "&")) != NULL) {
                    arguments[numArgs++] = strdup(argTokPtr);
                    tmp = NULL;
                }
                free(tmp);
                free(argTokPtr);
               
                // Setup array to pass for execution
                char * args[numArgs + 2];
                char prefix[15] = "http-root-dir\0";
                args[0] = strncat(prefix, docPath, questionPtr - docPath - 1);
                int i;
                for (i = 0 ; i < numArgs; i++) {
                    args[i + 1] = strdup(arguments[i]);
                }
                args[numArgs + 1] = NULL;
                
                // Write header to the client socket
                dup2(fd, 1);
                write(1, "HTTP/1.0 200 Document follows\015\012", 31);
                
                // Run the script. Output will go to client socket
                execvp(args[0], args);
                perror("execv");

                for (int i = 0; i < numArgs; i++) {
                    free(arguments[i]);
                }

                _exit(1);
            } else {
                // Write header to the client socket
                dup2(fd, 1);
                write(1, "HTTP/1.0 200 Document follows\015\012", 31);

                // Run the script. Output will go to client socket
                char * args[2];
                char prefix[15] = "http-root-dir\0";
                args[0] = strcat(prefix, docPath);
                args[1] = NULL;
                execvp(args[0], args);
                perror("execv");

                _exit(1);
            }
        } else if (pid < 0) {
            perror("fork");
        } else {
            waitpid(pid, NULL, 0);
        }

        // Restore stdout and close any open file descriptors
        dup2(tmpout, 1);
        close(tmpout);
        free(docPath);
        close(fd);
        return;
    } else {
        // Default: get document from htdocs directory
        strcat(filepath, "/http-root-dir/htdocs");
        strcat(filepath, docPath);
        free(docPath);
    }
    
    char ch;
    int reqFD;
    int hasError = 0;
    char errorMessage[32];
    char statusCode[5] = "200";
    char statusCodeMessage[32];
    char documentType[32];
    
    // Find where the file extension of requested doc begins
    char * filepathptr = filepath + strlen(filepath) - 1;
    while (filepathptr > filepath) {
        if (*filepathptr == '.') {
            break;
        }
        filepathptr--;
    }
    
    // Check the file extension and set the corresponding doctype
    if (strcmp(filepathptr, ".html") || strcmp(filepathptr, ".htm")) {
        strcpy(documentType, "text/html");
    } else if (strcmp(filepathptr, ".gif")) {
        strcpy(documentType, "image/gif");
    } else {
        strcpy(documentType, "text/plain");
    }

    // Check for invalid path when length of expanded path is
    // less than the length of cwd+"/http-root-dir"
    // Give 403 error for invalid path or 404 error for a file 
    // that could not be found or open
    if (strlen(filepath) < (strlen(cwd) + 14)) {
        hasError = 403;
        strcpy(statusCode, "403");
        strcpy(statusCodeMessage, " Forbidden");
        strcpy(errorMessage, " Forbidden");
        strcpy(documentType, "text/plain");
    } else if ((reqFD = open(filepath, O_RDONLY)) < 0) {
        hasError = 404;
        strcpy(statusCode, "404");
        strcpy(statusCodeMessage, " Not Found");
        strcpy(errorMessage, " File not found");
        strcpy(documentType, "text/plain");
    }

    free(filepath);
    free(cwd);

    if (hasError) {
        write(fd, "HTTP/1.0 ", 9);
        write(fd, statusCode, strlen(statusCode));
        write(fd, statusCodeMessage, strlen(statusCodeMessage));
        write(fd, "\015\012Server: ", 10);
        write(fd, serverType, strlen(serverType));
        write(fd, "\015\012", 2);
        write(fd, serverType, strlen(serverType));
        write(fd, " ", 1);
        write(fd, documentType, strlen(documentType));
        write(fd, "\015\012\015\012", 4);
        write(fd, errorMessage, strlen(errorMessage));
    } else {
        signal(SIGPIPE, SIG_IGN);
        write(fd, "HTTP/1.0 ", 9);
        write(fd, statusCode, strlen(statusCode));
        write(fd, " Document follows\015\012Server: ", 27);
        write(fd, serverType, strlen(serverType));
        write(fd, "\015\012Content-type: ", 15);
        write(fd, documentType, strlen(documentType));
        write(fd, "\015\012\015\012", 4);
        while (n = read(reqFD, &ch, sizeof(ch))) {
            if (write(fd, &ch, 1) != n) {
	            perror("write");
                break;
	        }
	    }
    }

    close(reqFD);
}

