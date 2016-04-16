void signalhandler(int signum) {
	if (signum == SIGTERM && !(flags & FLAG_SIGNAL)) {
		closelog();
		logerr("Zakonczono dzialanie demona. [kill]", 0);
		exit(signum);
	}
}

void sigusrhandler(int signum){
	flags |= FLAG_SIGNAL;
	perror("error");
}