/*
 * globals.h
 *
 *  Created on: 12 Apr 2016
 *      Author: jakez
 */

/**
 * WHY WE NEED MEMORY ALIGNMENT?
 * 1. Mips CPU 只能通过Load/Store两条指令访问内存
 * RISC的指令一般比较整齐，单条指令的功能单一，执行时间比较快。只能对寄存器中的数据运算，存储器的寻址一般只能通过L/S(Load/Store)进行。一般为等长指令，更便于流水线。
 * MIPS为RISC系统，等长指令，每条指令都有相同的长度：32位。其操作码固定为：6位。其余26位为若干个操作数。
 * 2. 内存地址的对齐
 * 对于一个32位的系统来说，CPU 一次只能从内存读32位长度的数据。如果CPU要读取一个int类型的变量并且该变量的起始位不在所读32位数据的首位，
 * 那么CPU肯定无法一次性读完这个变量，这时就说这个变量的地址是不对齐的。相反，如果CPU可以一次性读完一个变量，则说该变量的地址是对齐的。
 * 3. Mips CPU 要求内存地址（即Load/Store的操作地址）必须是对齐的
 * 其实不管是Mips，还是X86，都希望所操作地址是对齐的，因为这样可以最快速地处理数据。
 * 不过X86平台可以很容易很快速地处理不对齐的情况，而Mips一旦遇到地址不对齐的变量就会抛出exception,从而调用一大段后续处理代码，继而消耗大量的时间。
 * 因此，不管工作在什么平台下，程序员都应该养成使内存地址对齐的好习惯。
 */

#ifndef MY_GLOBALS_H_
#define MY_GLOBALS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <climits>
#include <assert.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <sys/types.h>
#endif

#ifdef __GNUC__
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#else
#define likely(x)       (x)
#define unlikely(x)    (x)
#endif

#ifdef __linux__
#include <endian.h>
# if __BYTE_ORDER == __LITTLE_ENDIAN
#endif
#endif

// for linux-kernal-systems
#ifdef __linux__
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef __FreeBSD__
#include <netinet/in_systm.h>
#include <sys/types.h>
#endif

#if defined(__sun)
# if defined(__svr4__)
/* Solaris */
#include <netinet/in_systm.h>
#include <stdarg.h>
# else
/* SunOS */
// add more files needed
# endif
#endif

#include "config.h"
#include "messages.h"

#include "geco-ds-malloc.h"
extern geco::ds::single_client_alloc galloc;

const uint OVERFLOW_SECS = (15 * 24 * 60 * 60);

/* ms default interval to timeout when no timers in poll
 * it is alos the resolution of wheel-timer*/
#define GRANULARITY 1

/* the maximum length of an IP address string (IPv4 or IPv6, NULL terminated) */
/* see RFC 1884 (mixed IPv6/Ipv4 addresses)   */
#define MAX_IPADDR_STR_LEN           46        /* ==  INET6_ADDRSTRLEN      */

// if our impl is based on UDP, this is the well-known-port 
// receiver and sender endpoints use 
#ifndef USED_UDP_PORT
#define USED_UDP_PORT 9899 //inna defined port
#endif

/* Define a protocol id to be used in the IP Header..... */
#ifndef IPPROTO_GECO
#define IPPROTO_GECO    132
#endif

#define USE_UDP_BUFSZ 65536 //RECV BUFFER IN POLLER
#define DEFAULT_RWND_SIZE  10*USE_UDP_BUFSZ // 655350 bytes =

/* src port + dest port + ver tag + checksum +
 chunk type + chunk flag + chunk length = 16 bytes*/
#define MIN_NETWORK_PACKET_HDR_SIZES \
GECO_PACKET_FIXED_SIZE+CHUNK_FIXED_SIZE
#define MAX_NETWORK_PACKET_HDR_SIZES 5552

//<--------------------------------- log ------------------------->
#define TRACE_MUDULE_SIZE 50
#define ENABLE_STR_LOG   false  /* set to != 0 if byte string logging should be done */

/* Definition of levels for the logging of events */
/* very VERBOSE logging of events   */
#define VVERBOSE           6
/* more VERBOSE logging of events   */
#define VERBOSE            5
/* pure execution flow trace */
#define INTERNAL_TRACE   4
/* important inernal events */
#define INTERNAL_EVENT  3
/* for events from ULP, peer or Timers */
#define EXTERNAL_TRACE   2
/* for unexpected external events from ULP, peer or Timers */
#define EXTERNAL_EVENT_UNEXPECTED 1
/* Defines the level up to which the events are prInt32ed.
 VVERBOSE (6) means all events are prInt32ed.
 This parameter could also come from a command line option */
#ifndef CURR_EVENT_LOG_LEVEL
#define CURR_EVENT_LOG_LEVEL 6
#endif

/* Definition of levels for the logging of errors */
/* warning, recovery not necessary. */
#define WARNNING_ERROR 4
/* recovery from error was possible without affecting the system. */
#define MINOR_ERROR  3
/*recovery from error was possible with some affects to the system,
 * for instance abort of an association.*/
#define MAJOR_ERROR  2
/* recovery from error was not possible, the program exits. */
#define FALTAL_ERROR_EXIT 1
/* Defines the level up to which the errors are prInt32ed.
 *ERROR_WARNING (4) means all events are prInt32ed.
 *This parameter could also come from a command line option*/
#define CURR_ERROR_LOG_LEVEL 4

#define EVENTLOG(x,y)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y))
#define EVENTLOG1(x,y,z)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z))
#define EVENTLOG2(x,y,z,i)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i))
#define EVENTLOG3(x,y,z,i,j)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j))
#define EVENTLOG4(x,y,z,i,j,k)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j),(k))
#define EVENTLOG5(x,y,z,i,j,k,l)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j),(k),(l))
#define EVENTLOG6(x,y,z,i,j,k,l,m)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j),(k),(l),(m))
#define EVENTLOG7(x,y,z,i,j,k,l,m,n)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j),(k),(l),(m),(n))
#define EVENTLOG8(x,y,z,i,j,k,l,m,n,o)\
if (CURR_EVENT_LOG_LEVEL >= x) event_log1((x), __FILE__, (y), (z), (i), (j),(k),(l),(m),(n),(o))

#define ERRLOG(x,y)  \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y))
#define ERRLOG1(x,y,z)\
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y),(z))
#define ERRLOG2(x,y,z,i)   \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y),(z),(i))
#define ERRLOG3(x,y,z,i,j)     \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y),(z),(i),(j))
#define ERRLOG4(x,y,z,i,j,k)    \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y),(z),(i),(j),(k))
#define ERRLOG5(x,y,z,i,j,k,m)    \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y),(z),(i),(j),(k), (m))

#define ERRLOGSYS(x,y)    \
error_log_sys1((x), __FILE__, __LINE__, (y))

#define ERRLOGDLL(x,y)    \
if (CURR_ERROR_LOG_LEVEL >= x) error_log1((x), __FILE__, __LINE__, (y))

#define IFLOG(x, y)       \
if (x <= CURR_ERROR_LOG_LEVEL) {y}

#ifdef LIBRARY_DEBUG
#define ENTER_LIBRARY(fname)	printf("Entering sctplib  (%s)\n", fname); fflush(stdout);
#define LEAVE_LIBRARY(fname)	printf("Leaving  sctplib  (%s)\n", fname); fflush(stdout);
#define ENTER_CALLBACK(fname)	printf("Entering callback (%s)\n", fname); fflush(stdout);
#define LEAVE_CALLBACK(fname)	printf("Leaving  callback (%s)\n", fname); fflush(stdout);
#el
#define ENTER_LIBRARY(fname)
#define LEAVE_LIBRARY(fname)
#define ENTER_CALLBACK(fname)
#define LEAVE_CALLBACK(fname)
#endif

#ifdef __DEBUG
#define ENTER_TIMER_DISPATCHER printf("Entering timer dispatcher.\n"); fflush(stdout);
#define LEAVE_TIMER_DISPATCHER printf("Leaving  timer dispatcher.\n"); fflush(stdout);
#define ENTER_EVENT_DISPATCHER printf("Entering event dispatcher.\n"); fflush(stdout);
#define LEAVE_EVENT_DISPATCHER printf("Leaving  event dispatcher.\n"); fflush(stdout);
#else
#define ENTER_TIMER_DISPATCHER
#define LEAVE_TIMER_DISPATCHER
#define ENTER_EVENT_DISPATCHER
#define LEAVE_EVENT_DISPATCHER
#endif
/**
 *read_tracelevels reads from a file the tracelevels for errors and events for each module.
 *Modules that are not listed in the file will not be traced. if the file does not exist or
 *is empty, the global tracelevel defined in globals.h will be used. THe name of the file has
 *to be {\texttt tracelevels.in} in the current directory where the executable is located.
 *The normal format of the file is:
 *\begin{verbatim}
 *module1.c errorTraceLevel eventTraceLevel
 *module2.c errorTraceLevel eventTraceLevel
 *....
 *\end{verbatim}
 *The file must be terminated by a null line.
 *Alternatively there may be the entry
 *\begin{verbatim}
 * LOGFILE
 *\end{verbatim}
 * in that file, which causes all output from event_logs() to go into a logfile in the local
 * directory.
 */
extern void read_trace_levels(void);
// print fixed date and then the msg
extern void debug_print(FILE * fd, const char *f, ...);

/**
 * print the error string after a system call and exit
 perror和strerror都是C语言提供的库函数，用于获取与erno相关的错误信息，区别不大，
 用法也简单。最大的区别在于perror向stderr输出结果，而 strerror向stdout输出结果。
 perror ( )用 来 将 上 一 个 函 数 发 生 错 误 的 原 因 输 出 到 标 准 设备 (stderr) 。
 参数 s 所指的字符串会先打印出,后面再加上错误原因字符串。
 此错误原因依照全局变量error 的值来决定要输出的字符串。
 在库函数中有个error变量，每个error值对应着以字符串表示的错误类型。
 当你调用"某些"函数出错时，该函数已经重新设置了error的值。
 perror函数只是将你输入的一些信息和现在的error所对应的错误一起输出。
 stderror 只向屏幕输出， 但是stdout可以被重定向到各种输出设备， 如文件 等
 see http://www.cnblogs.com/zhangyabin---acm/p/3203745.html
 http://blog.csdn.net/lalor/article/details/7555019
 */
extern void perr_exit(const char *infostring);
extern void perr_abort(const char *infostring);

/* This function logs events.
 Parameters:
 @param event_loglvl : INTERNAL_EVENT_0 INTERNAL_EVENT_1 EXTERNAL_EVENT_X EXTERNAL_TRACE
 @param module_name :     the name of the module that received the event.
 @param log_info :        the info that is prInt32ed with the modulename.
 @param anyno :           optional poInt32er to uint, which is prInt32ed along with log_info.
 The conversion specification must be contained in log_info.
 @author     H�zlwimmer
 */
extern void event_log1(short event_loglvl, const char *module_name,
        const char *log_info, ...);

/* This function logs errors.
 Parameters:
 @param error_loglvl : ERROR_MINOR ERROR_MAJOR ERROR_FATAL
 @param module_name :     the name of the module that received the event.
 @param line_no :         the line number within above module.
 @param log_info :        the info that is prInt32ed with the modulename.
 @author     H�zlwimmer
 */
extern void error_log1(short error_loglvl, const char *module_name, int line_no,
        const char *log_info, ...);

/* This function logs system call errors.
 This function calls ERRLOG.
 Parameters:
 @param error_loglvl : ERROR_MINOR ERROR_MAJOR ERROR_FATAL
 @param module_name :     the name of the module that received the event.
 @param line_no :         the line number within above module.
 @param errnumber :       the errno from systemlibrary.
 @param log_info :        the info that is prInt32ed with the modulename and error text.
 @author     H�zlwimmer
 */
extern void error_log_sys1(short error_loglvl, const char *module_name,
        int line_no, short errnumber);

//<---------------- time-------------------->
typedef uint TimerID;
#define   TIMER_TYPE_INIT       0
#define   TIMER_TYPE_SHUTDOWN   1
#define   TIMER_TYPE_RTXM       3
#define   TIMER_TYPE_SACK       2
#define   TIMER_TYPE_CWND       4
#define   TIMER_TYPE_HEARTBEAT  5
#define   TIMER_TYPE_USER       6
#define   MAX(a,b) (a>b)?(a):(b)

/*helper to init timeval struct with ms interval*/
#define fills_timeval(timeval_ptr, time_t_inteval)\
(timeval_ptr)->tv_sec = (time_t_inteval) / 1000;\
(timeval_ptr)->tv_usec = ((time_t_inteval) % 1000) * 1000;

extern void sum_time(timeval* a, timeval* b, timeval* result);
extern void sum_time(timeval* a, time_t inteval/*ms*/, timeval* result);
extern void subtract_time(timeval* a, timeval* b, timeval* result);
extern int subtract_time(timeval* a, timeval* b); //return time different as ms
extern void subtract_time(timeval* a, time_t inteval/*ms*/, timeval* result);
//the_time reply on timeval and so for high efficicy, you will be always be given 
// timeval when you need date calling second getitmenow
extern int gettimenow(struct timeval *tv);
extern int gettimenow(struct timeval *tv, struct tm *the_time);
extern int gettimenow_ms(time_t* ret);
extern int gettimenow_us(time_t* ret);
// function to output the result of the get_time_now-call, i.e. the time now
extern void print_time_now(ushort level);
extern void print_timeval(timeval* tv);

//<---------------------- helpers --------------------->
enum ctrl_type
{
    bundle_ctrl,
    recv_ctrl,
    flow_ctrl,
    reliable_transfer_ctrl,
    path_ctrl,
    geco_ctrl,
    stream_ctrl,
    unkown
};

struct internal_stream_data_t
{
        ushort stream_id;
        ushort stream_sn;
};
struct internal_data_chunk_t
{
        uint chunk_len;
        uint chunk_tsn; /* for efficiency */
        uchar data[MAX_NETWORK_PACKET_VALUE_SIZE];

        uint gap_reports;

        struct timeval transmission_time;
        /* ack_time : in msecs after transmission time, initially 0, -1 if retransmitted */
        int ack_time;
        uint num_of_transmissions;

        /* time after which chunk should not be retransmitted */
        struct timeval expiry_time;
        bool dontBundle;

        /* lst destination used to send chunk to */
        uint last_destination;
        int initial_destination;

        /* this is set to true, whenever chunk is sent/received on unreliable stream */
        bool isUnreliable;

        bool hasBeenAcked;
        bool hasBeenDropped;
        bool hasBeenFastRetransmitted;
        bool hasBeenRequeued;
        bool context;

        /*which ctrl this struct belongs to*/
        ctrl_type ct;
};

/**
 * helper functions that correctly handle overflowed issue.
 * int溢出超出了int类型的最大值，如果是两个正数相加，溢出得到一个负数，
 * 或两个负数相加，溢出得到一个正数的情况，就叫溢出。
 * 或者两个整数相减，溢出得到与实际不相符的结果，都叫溢出问题
 * 总结一下：
 * 获取与编译器相关的int、char、long的最大值的方法分别为
 * 　1） 使用头文件 <limits.h> 里面分别有关于最大、最小的char 、int、long的。
 * 2） 分别将-1转换成对应的unsigned char 、unsigned int、unsigned long值
 */
extern bool safe_before(uint seq1, uint seq2);
extern bool safe_after(uint seq1, uint seq2);
extern bool safe_before(ushort seq1, ushort seq2);
extern bool safe_after(ushort seq1, ushort seq2);

// if s1 <= s2 <= s3
// @pre seq1 <= seq3
extern bool safe_between(uint seq1, uint seq2, uint seq3);
// @pre make sure seq1 <= seq3
extern bool unsafe_between(uint seq1, uint seq2, uint seq3);

/**
 * compute IP checksum yourself. If packet does not have even packet boundaries,
 * last byte will be set 0 and length increased by one. (should never happen in
 * this SCTP implementation, since we always have 32 bit boundaries !
 * Make sure the checksum is computed last thing before sending, and the checksum
 * field is initialized to 0 before starting the computation
 */
extern ushort in_check(uchar *buf, int sz);
int sort_ssn(const internal_stream_data_t& one,
        const internal_stream_data_t& two);
// function that correctly sorts TSN values, minding wrapround
extern int sort_tsn(const internal_data_chunk_t& one,
        const internal_data_chunk_t& two);

/*=========== help functions =================*/
#define BITS_TO_BYTES(x) (((x)+7)>>3)
extern char* Bitify(size_t mWritePosBits, char* mBuffer);
extern void Bitify(char* out, size_t mWritePosBits, char* mBuffer);

/*================ sockaddr defines and functions =================*/
#ifndef IN_EXPERIMENTAL
#define  IN_EXPERIMENTAL(a)   ((((int) (a)) & 0xf0000000) == 0xf0000000)
#endif

#ifndef IN_BADCLASS
#define  IN_BADCLASS(a)    IN_EXPERIMENTAL((a))
#endif

#define s4addr(X)   (((struct sockaddr_in *)(X))->sin_addr.s_addr)
#define sin4addr(X)   (((struct sockaddr_in *)(X))->sin_addr)
#define s6addr(X)  (((struct sockaddr_in6 *)(X))->sin6_addr.s6_addr)
#define sin6addr(X)  (((struct sockaddr_in6 *)(X))->sin6_addr)
#define saddr_family(X)  (X)->sa.sa_family

#define SUPPORT_ADDRESS_TYPE_IPV4        0x00000001
#define SUPPORT_ADDRESS_TYPE_IPV6        0x00000002
#define SUPPORT_ADDRESS_TYPE_DNS         0x00000004

#define DEFAULT_MTU_CEILING     1500

// SEE http://book.51cto.com/art/201012/236880.htm
// USE MINIMUM_DELAY AS TOS
#define IPTOS_DEFAULT (0xe0|0x1000) // Precedence 111 + TOS 1000 + MBZ 0

//typedef enum {
//      flag_HideLoopback           = (1 << 0),
//      flag_HideLinkLocal          = (1 << 1),
//      flag_HideSiteLocal          = (1 << 2),
//      flag_HideLocal              = flag_HideLoopback|flag_HideLinkLocal|flag_HideSiteLocal,
//      flag_HideAnycast            = (1 << 3),
//      flag_HideMulticast          = (1 << 4),
//      flag_HideBroadcast          = (1 << 5),
//      flag_HideReserved           = (1 << 6),
//      flag_Default                = flag_HideBroadcast|flag_HideMulticast|flag_HideAnycast,
//      flag_HideAllExceptLoopback  = (1 << 7),
//      flag_HideAllExceptLinkLocal = (1 << 8),
//      flag_HideAllExceptSiteLocal = (1 << 9)
//} AddressScopingFlags;
enum IPAddrType
{
    LoopBackAddrType = (1 << 0),
    LinkLocalAddrType = (1 << 1),
    SiteLocalAddrType = (1 << 2),
    AnyCastAddrType = (1 << 3),
    MulticastAddrType = (1 << 4),
    BroadcastAddrType = (1 << 5),
    ReservedAddrType = (1 << 6),
    AllExceptLoopbackAddrTypes = (1 << 7),
    AllExceptLinkLocalAddrTypes = (1 << 8),
    ExceptSiteLocalAddrTypes = (1 << 9),
    //flag_Default
    AllCastAddrTypes = BroadcastAddrType | MulticastAddrType | AnyCastAddrType,
    //flag_HideLocal
    AllLocalAddrTypes = LoopBackAddrType | LinkLocalAddrType | SiteLocalAddrType,
};

/* union for handling either type of addresses: ipv4 and ipv6 */
union sockaddrunion
{
        struct sockaddr sa;
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
};

// key of channel
struct transport_addr_t
{
        sockaddrunion local_saddr;
        sockaddrunion peer_saddr;
};

/* converts address-string
 * (hex for ipv6, dotted decimal for ipv4 to a sockaddrunion structure)
 *  str == NULL will bitzero saddr used as 'ANY ADRESS 0.0.0.0'
 *  port number will be always >0
 *  default  is IPv4
 *  @return 0 for success, else -1.*/
extern int str2saddr(sockaddrunion *su, const char * str, ushort port = 0,
        bool ip4 = true);
extern int saddr2str(sockaddrunion *su, char * buf, size_t len,
        ushort* portnum = NULL);
inline extern bool saddr_equals(sockaddrunion *a, sockaddrunion *b,
        bool ignore_port = false)
{
//	EVENTLOG2(VERBOSE, "a af%d, b af%d\n", a->sin.sin_family,
//			b->sin.sin_family);
    switch (saddr_family(a))
    {
        case AF_INET:
            return saddr_family(b) == AF_INET &&
            s4addr(&a->sin) == s4addr(&b->sin)
                    && (ignore_port || a->sin.sin_port == b->sin.sin_port);
            break;
        case AF_INET6:
            return saddr_family(b) == AF_INET6
                    && (ignore_port || a->sin6.sin6_port == b->sin6.sin6_port)
                    && (memcmp(s6addr(&a->sin6), s6addr(&b->sin6), 16) == 0);
            break;
        default:
            ERRLOG1(MAJOR_ERROR, "Address family %d not supported",
                    saddr_family(a));
            return false;
            break;
    }
}

//! From http://www.azillionmonkeys.com/qed/hash.html
//! Author of main code is Paul Hsieh. I just added some convenience functions
//! Also note http://burtleburtle.net/bob/hash/doobs.html, which shows that this is 20%
//! faster than the one on that page but has more collisions
extern unsigned long SuperFastHash(const char * data, int length);
extern unsigned long SuperFastHashIncremental(const char * data, int len,
        unsigned int lastHash);
extern unsigned long SuperFastHashFile(const char * filename);
extern unsigned long SuperFastHashFilePtr(FILE *fp);
extern unsigned int transportaddr2hashcode(const sockaddrunion* local_sa,
        const sockaddrunion* peer_sa);
extern unsigned int sockaddr2hashcode(const sockaddrunion* sa);

/*=========  DISPATCH LAYER  LAYER DEFINES AND FUNTIONS ===========*/
#define ASSOCIATION_MAX_RETRANS_ATTEMPTS 10
#define MAX_INIT_RETRANS_ATTEMPTS    8
#define MAX_PATH_RETRANS_TIMES    5
#define VALID_COOKIE_LIFE_TIME  10000 //MS

#define SACK_DELAY    200
#define RTO_INITIAL     3000    /* 超时重传机制(RTO：Retransmission Timeout) */
#define RTO_MIN                 1000
#define RTO_MAX                 60000

#define DEFAULT_MAX_SENDQUEUE   0       /* unlimited send queue */
#define DEFAULT_MAX_RECVQUEUE   0       /* unlimited recv queue - unused really */
#define DEFAULT_MAX_BURST       4       /* maximum burst parameter */
#define DEFAULT_ENDPOINT_SIZE 128

/*this parameter specifies the maximum number of addresses
 that an endpoint may have */
#define MAX_NUM_ADDRESSES      32

/*====== APPLICATION LAYER DEFINES AND FUNTIONS =======*/

/**
 This struct containes the pointers to ULP callback functions.
 Each SCTP-instance can have its own set of callback functions.
 The callback functions of each SCTP-instance can be found by
 first reading the datastruct of an association from the list of
 associations. The datastruct of the association contains the name
 of the SCTP instance to which it belongs. With the name of the SCTP-
 instance its datastruct can be read from the list of SCTP-instances.
 */
struct applicaton_layer_cbs_t
{
        /* @{ */
        /**
         * indicates that new data arrived from peer (chapter 10.2.A).
         *  @param 1 associationID
         *  @param 2 streamID
         *  @param 3 length of data
         *  @param 4 stream sequence number
         *  @param 5 tsn of (at least one) chunk belonging to the message
         *  @param 6 protocol ID
         *  @param 7 unordered flag (TRUE==1==unordered, FALSE==0==normal, numbered chunk)
         *  @param 8 pointer to ULP data
         */
        void (*dataArriveNotif)(unsigned int, unsigned short, unsigned int,
                unsigned short, unsigned int, unsigned int, unsigned int,
                void*);
        /**
         * indicates a send failure (chapter 10.2.B).
         *  @param 1 associationID
         *  @param 2 pointer to data not sent
         *  @param 3 dataLength
         *  @param 4 pointer to context from sendChunk
         *  @param 5 pointer to ULP data
         */
        void (*sendFailureNotif)(unsigned int, unsigned char *, unsigned int,
                unsigned int *, void*);
        /**
         * indicates a change of network status (chapter 10.2.C).
         *  @param 1 associationID
         *  @param 2 destinationAddresses
         *  @param 3 newState
         *  @param 4 pointer to ULP data
         */
        void (*networkStatusChangeNotif)(unsigned int, short, unsigned short,
                void*);
        /**
         * indicates that a association is established (chapter 10.2.D).
         *  @param 1 associationID
         *  @param 2 status, type of event
         *  @param 3 number of destination addresses
         *  @param 4 number input streamns
         *  @param 5 number output streams
         *  @param 6 int  supportPRSCTP (0=FALSE, 1=TRUE)
         *  @param 7 pointer to ULP data, usually NULL
         *  @return the callback is to return a pointer, that will be transparently returned with every callback
         */
        void* (*communicationUpNotif)(unsigned int, int, unsigned int,
                unsigned short, unsigned short, int, void*);
        /**
         * indicates that communication was lost to peer (chapter 10.2.E).
         *  @param 1 associationID
         *  @param 2 status, type of event
         *  @param 3 pointer to ULP data
         */
        void (*communicationLostNotif)(unsigned int, unsigned short, void*);
        /**
         * indicates that communication had an error. (chapter 10.2.F)
         * Currently not implemented !?
         *  @param 1 associationID
         *  @param 2 status, type of error
         *  @param 3 pointer to ULP data
         */
        void (*communicationErrorNotif)(unsigned int, unsigned short, void*);
        /**
         * indicates that a RESTART has occurred. (chapter 10.2.G)
         *  @param 1 associationID
         *  @param 2 pointer to ULP data
         */
        void (*restartNotif)(unsigned int, void*);
        /**
         * indicates that a SHUTDOWN has been received by the peer. Tells the
         * application to stop sending new data.
         *  @param 0 instanceID
         *  @param 1 associationID
         *  @param 2 pointer to ULP data
         */
        void (*peerShutdownReceivedNotif)(unsigned int, void*);
        /**
         * indicates that a SHUTDOWN has been COMPLETED. (chapter 10.2.H)
         *  @param 0 instanceID
         *  @param 1 associationID
         *  @param 2 pointer to ULP data
         */
        void (*shutdownCompleteNotif)(unsigned int, void*);
        /**
         * indicates that a queue length has exceeded (or length has dropped
         * below) a previously determined limit
         *  @param 0 associationID
         *  @param 1 queue type (in-queue, out-queue, stream queue etc.)
         *  @param 2 queue identifier (maybe for streams ? 0 if not used)
         *  @param 3 queue length (either bytes or messages - depending on type)
         *  @param 4 pointer to ULP data
         */
        void (*queueStatusChangeNotif)(unsigned int, int, int, int, void*);
        /**
         * indicates that a ASCONF request from the ULP has succeeded or failed.
         *  @param 0 associationID
         *  @param 1 correlation ID
         *  @param 2 result (int, negative for error)
         *  @param 3 pointer to a temporary, request specific structure (NULL if not needed)
         *  @param 4 pointer to ULP data
         */
        void (*asconfStatusNotif)(unsigned int, unsigned int, int, void*,
                void*);
        /* @} */
};

#endif /* MY_GLOBALS_H_ */