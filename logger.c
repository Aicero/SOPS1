void logger(char* message) {
	time_t currentTime;
    currentTime = time(NULL);
	syslog(LOG_INFO, "%s", message);
	if(verbose)fprintf(stderr, "%s %s",ctime(&currentTime), message);
}