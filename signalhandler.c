void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog();
		loggererr("Zakonczono dzialanie demona. [kill]", 0);
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		g_flagaSignal = 1;
	}
}