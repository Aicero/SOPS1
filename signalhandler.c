void signalhandler(int signum) {
	if (signum == SIGTERM) {
		flags |= FLAG_KILL;
	}
}

void sigusrhandler(int signum) {
	flags |= FLAG_SIGNAL;
}
