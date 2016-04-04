void logger(char* message) {
	syslog(LOG_INFO, "%s", message);
	//fprintf(stderr, "\n%s", message);
}