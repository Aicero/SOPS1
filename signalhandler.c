void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog();
		logerr("Zakonczono dzialanie demona. [kill]", 0);
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		g_flagSignal = 1;
	}
}