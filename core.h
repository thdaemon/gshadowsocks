#ifndef __CORE_H_
#define __CORE_H_

int core_server_del(const char *srvname);
int core_server_add(const char *srvname, const char *srvaddr,
		    const char *srvport, const char *localport,
		    const char *passwd, const char *method);
int core_server_exists(const char *srvname);

#endif /* __CORE_H_ */
