#!/bin/sh
# This is an example script that runs the irc proxy.

## Edit these values
export PROXY_PASS="s4cr3t-and-secure"
IRC_SERVER="irc.xinutec.net"
IRC_PORT="6697" # which port does the IRC server listen for SSL?
LISTEN_PORT="6667" # listen on localhost:6667
SSL_CA="xinutec-ca.crt"
SSL_KEY="combined.pem"
SSL_CERT="combined.pem" # This assumes that both the key and certificate are concatenated to one file using e.g. cat key.pem cert.pem > combined.pem

# stop editing unless you know what you're doing.

exec socat "TCP-LISTEN:$LISTEN_PORT,bind=localhost,fork,reuseaddr" \
	SYSTEM:"exec ./ircproxy -c $SSL_CERT -k $SSL_KEY -C $SSL_CA -s $IRC_SERVER\:$IRC_PORT"
