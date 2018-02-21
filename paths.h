#ifndef __PATHS_H_
#define __PATHS_H_

#include "config.h"

#define SRVWIN_GLADE_FILE_PATH CONFIG_PREFIX "/share/gshadowsocks/srvwin.glade"
#define EDITWIN_GLADE_FILE_PATH CONFIG_PREFIX "/share/gshadowsocks/editwin.glade"
#define PROXY_HELPER_PATH CONFIG_PREFIX "/lib/gshadowsocks/proxy-helper"
#define ICON_FILE_NORMAL CONFIG_PREFIX "/share/gshadowsocks/icon/normal.png"
#define ICON_FILE_CONN CONFIG_PREFIX "/share/gshadowsocks/icon/conn.png"

extern char *workdir;
extern char *srvdir;
extern char *conff;

void path_init();

#endif /* __PATHS_H_ */
