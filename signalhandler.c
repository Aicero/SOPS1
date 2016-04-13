void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog();
		syslog(LOG_NOTICE, "Zakonczono dzialanie demona. [kill]");
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		g_flagaSignal = 1;
	}
}