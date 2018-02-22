#ifndef __SRVWIN_H_
#define __SRVWIN_H_

char *methods[] = {
	"rc4-md5",
	"aes-128-gcm",
	"aes-192-gcm",
	"aes-256-gcm",
	"aes-128-cfb",
	"aes-192-cfb",
	"aes-256-cfb",
	"aes-128-ctr",
	"aes-192-ctr",
	"aes-256-ctr",
	"camellia-128-cfb",
	"camellia-192-cfb",
	"camellia-256-cfb",
	"bf-cfb",
	"chacha20-ietf-poly1305",
	"salsa20",
	"chacha20",
	"chacha20-ietf"
};

void srvwin_show();

#endif /* __SRVWIN_H_ */
