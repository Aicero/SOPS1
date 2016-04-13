void logger(char* message) {
	time_t currentTime;
	currentTime = time(NULL);
	syslog(LOG_INFO, "%s", message);
	if (g_verbose) fprintf(stderr, "%s %s", ctime(&currentTime), message);
}
void loggerparam(char* message, char* sParam){
	time_t currentTime;
	currentTime = time(NULL);
	syslog(LOG_INFO, "%s %s", message, sParam);
	if (g_verbose) fprintf(stderr, "%s %s %s", ctime(&currentTime), message, sParam);
}