void logger(char* message) {
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");
	syslog(LOG_INFO, "%s", message);
	if (g_verbose) fprintf(stderr, "%s %s\n", currentTime, message);
}
void loggerparam(char* message, char* sParam){
	time_t cTime;
	cTime = time(NULL);

	char *currentTime;
	currentTime = strtok(ctime(&cTime), "\n");
	syslog(LOG_INFO, "%s %s", message, sParam);
	if (g_verbose) fprintf(stderr, "%s %s\n Parametr: %s\n\n", currentTime, message, sParam);
}