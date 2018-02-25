#ifndef __CORE_H_
#define __CORE_H_

int core_server_del(const char *srvname);
int core_server_add(const char *srvname, const char *srvaddr,
		    const char *srvport, const char *localport,
		    const char *passwd, const char *method);
int core_server_exists(const char *srvname);
int core_sconf_gen(const char *srvname);
int core_start_sslocal();
void core_stop_sslocal();

#define PROXY_HELPER_OPCODE_CLEAR -1
#define PROXY_HELPER_OPCODE_PAC 0
#define PROXY_HELPER_OPCODE_GLOBAL 1

int core_pac_update(const char *srvname);
int core_proxy_helper(int opcode, const char *srvname);

#endif /* __CORE_H_ */
