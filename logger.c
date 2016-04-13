void logger(char* message) {
	syslog(LOG_INFO, "%s", message);
	if(verbose)fprintf(stderr, "%s", message);
}