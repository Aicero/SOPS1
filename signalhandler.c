void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog(); // zamkniecie logow
		syslog(LOG_NOTICE, "Zakonczono dzialanie demona. [kill]");
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		flagaSignal = 1;
	}
}