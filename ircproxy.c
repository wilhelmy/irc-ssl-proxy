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

#include <unistd.h>
#include <stdlib.h>
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
		DIE("Please put your password in the PROXY_PASS environment variable");

	while ((c = getopt (argc, argv, "c:C:k:s:h")) != -1) {
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
		case 'h':
		default:
			DIE("Please consult the manpage for help");
		}
	}
	if (!hostport) DIE("No host+port given (parameter -s).");
	if (!ca)       DIE("No CA certificate given (parameter -C)");
	if (!cert)     DIE("No SSL client certificate given (parameter -c)");
	if (!key)      DIE("No SSL private key given (parameter -k)");
	arg[4] = hostport;
	arg[6] = ca;
	arg[8] = cert;
	arg[10]= key;

	fgets(buf, sizeof(buf), stdin);
	if (!strncasecmp(buf, "PASS ", 5)) {
		size_t i = 5;
		if (buf[i] == ':') i++;
		strtok(buf+i,"\r\n");
		if (strcmp(buf+i, pass)) {
			fputs("ERROR : wrong password\r\n", stdout);
			DIE("wrong password");
		}
	} else {
		fputs("ERROR : wrong password\r\n", stdout);
		DIE("wrong password");
	}
	execvp(arg[0], arg);
	DIE("execvp failed");
}
