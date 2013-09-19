/* Copyright (c) 2013 Moritz Wilhelmy. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Usage:
	# compile: cc -o ircproxy ircproxy.c

	# then execute the following shellscript:
	export PROXY_PASS=s4cr3t
	socat TCP-LISTEN:6667,bind=localhost,fork,reuseaddr SYSTEM:'exec ./ircproxy -c combined.pem -k combined.pem -C xinutec-ca.crt -s irc.xinutec.net\:6697'
 */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#define DIE(x) (fprintf(stderr, x"\n"),exit(1))

static char *arg[] = {
	"openssl", "s_client", "-quiet", "-connect", NULL, "-CAfile", NULL, "-cert", NULL, "-key", NULL, NULL
};

int
main(int argc, char **argv)
{
	char c;
	char *hostport = NULL, *ca = NULL, *cert = NULL, *key = NULL;
	char buf[1024];
	char *pass = getenv("PROXY_PASS");

	if (!pass)
		DIE("Please put your password in PROXY_PASS");

	while ((c = getopt (argc, argv, "c:C:k:s:")) != -1) {
		switch (c) {
		case 's':
			hostport = optarg;
			break;
		case 'k':
			key = optarg;
			break;
		case 'C':
			ca = optarg;
			break;
		case 'c':
			cert = optarg;
			break;
		default:
			exit(1); // FIXME help text
		}
	}
	if (!(hostport && ca && cert && key))
		DIE("one was undefined");
	arg[4] = hostport;
	arg[6] = ca;
	arg[8] = cert;
	arg[10]= key;

	fgets(buf, sizeof(buf), stdin);
	if (!strncasecmp(buf, "PASS ", 5)) {
		size_t i = 5;
		if (buf[i] == ':') i++;
		strtok(buf,"\n");
		strtok(buf,"\r");
		if (strcmp(buf+i, pass)) {
			puts("ERROR : wrong password\r");
			fflush(stdout);
			DIE("wrong password");
		}
	} else {
		puts("ERROR : wrong password\r");
		fflush(stdout);
		DIE("wrong password");
	}
	execvp(arg[0], arg);
	DIE("execvp failed");
}
