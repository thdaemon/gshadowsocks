#ifndef __PATHS_H_
#define __PATHS_H_

#define SRVWIN_GLADE_FILE_PATH "./share/main.glade"
#define EDITWIN_GLADE_FILE_PATH "./share/main.glade"
#define PROXY_HELPER_PATH "/usr/lib/gshadowsocks/proxy-helper"
#define ICON_FILE_NORMAL "/media/th/disk2/th/project/gshadowsocks/share/icon/normal.png"
#define ICON_FILE_CONN "/usr/share/gshadowsocks/icon/conn.png"

extern char *workdir;
extern char *srvdir;
extern char *conff;

void path_init();

#endif /* __PATHS_H_ */
