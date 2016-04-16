/*
* Logowanie zdarzen z rodzajem bledu (jezeli jakis byl)
*/

void logerr(const char* message,const int err) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");

	if (err != 0) {
		syslog(LOG_INFO, "%s error: %s", message, strerror(err));
		if (flags & VERBOSE) fprintf(stderr, "%s %s\n error: %s\n", message, currentTime, strerror(err));
	}
	else {
		syslog(LOG_INFO, "%s", message);
		if (flags & VERBOSE) fprintf(stderr, "%s %s\n", message, currentTime);
	}
}

/*
*	Logowanie zdarzen z dodatkowa informacja (sParam) oraz z rodzajem bledu (jezeli jakis byl)
*/

void logparamerr(const char* message,const char* sParam,const int err) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");

	if (err != 0) {
		syslog(LOG_INFO, "%s (%s) error: %s", message, sParam, strerror(err));
		if (flags & VERBOSE) fprintf(stderr, "%s %s\n\tParametr: %s\n\t error: %s\n", sParam, message, currentTime, strerror(err));
	}
	else {
		syslog(LOG_INFO, "%s (%s)", message, sParam);
		if (flags & VERBOSE) fprintf(stderr, "%s %s\n Parametr: %s\n", message, currentTime, sParam);
	}
}