void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog(); // zamkniecie logow
		syslog(LOG_NOTICE, "Zakonczono dzialanie demona. [kill]");
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		// wywolanie metody synchronizujacej katalogi
		logger("Demon wybudzony przez SIGUSR1.");
		sleep(refreshtime); /* uspienie procesu */
	}
}