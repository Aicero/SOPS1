void logger(char* message) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");
	syslog(LOG_INFO, "-----%s", message);
	if (g_verbose) fprintf(stderr, "-----%s %s\n", currentTime, message);
}
void loggerparam(char* message, char* sParam) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");
	syslog(LOG_INFO, "-----%s %s", message, sParam);
	if (g_verbose) fprintf(stderr, "-----%s %s\n Parametr: %s\n\n", currentTime, message, sParam);
}

//---------------------------------
void loggererr(char* message, int err) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");

	if (err != 0) {
		syslog(LOG_INFO, "%s error: %s", message, strerror(err));
		if (g_verbose) fprintf(stderr, "%s %s\n\terror: %s\n", currentTime, message, strerror(err));
	}
	else {
		syslog(LOG_INFO, "%s", message);
		if (g_verbose) fprintf(stderr, "%s %s\n", currentTime, message);
	}
}

void loggerparamerr(char* message, char* sParam, int err) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");

	if (err != 0) {
		syslog(LOG_INFO, "%s %s error: %s", message, sParam, strerror(err));
		if (g_verbose) fprintf(stderr, "%s %s\n\tParametr: %s\n\t error: %s\n", currentTime, message, strerror(err), sParam);
	}
	else {
		syslog(LOG_INFO, "%s %s", message, sParam);
		if (g_verbose) fprintf(stderr, "%s %s\n\tParametr: %s\n", currentTime, message, sParam);
	}
}