#include "gtest/gtest.h"
#include "gmock/gmock.h"
// @caution because geco-ds-malloc includes geco-thread.h that includes window.h but transport_layer.h includes wsock2.h, as we know, it must include before windows.h so if you uncomment this line, will cause error
//#include "geco-ds-malloc.h"
#include "transport_layer.h"
#include "globals.h"
#include "geco-ds-malloc.h"
#include "geco-malloc.h"
using namespace geco::ds;

#include "config.h"

#ifdef _WIN32
#define mysleep Sleep
#else
#define mysleep sleep
#endif

extern timer_mgr mtra_timer_mgr_;
extern int mtra_icmp_rawsock_; /* socket fd for ICMP messages */
extern int socket_despts_size_;
extern socket_despt_t socket_despts[MAX_FD_SIZE];
extern event_handler_t event_callbacks[MAX_FD_SIZE];

extern void
mtra_set_expected_event_on_fd (int fd_index, int sfd, int event_mask);
extern void
mtra_set_expected_event_on_fd (int sfd, int eventcb_type, int event_mask,
                               cbunion_t action, void* userData);
extern void
mtra_add_stdin_cb (stdin_data_t::stdin_cb_func_t stdincb);
extern int
mtra_poll (void
(*lock) (void* data),
           void
           (*unlock) (void* data),
           void* data);
extern int
mtra_remove_stdin_cb ();
extern int
mtra_remove_event_handler (int sfd);
extern int
mtra_read_ip4rawsock ();
extern int
mtra_read_ip6rawsock ();
extern int
mtra_read_icmp_socket ();
extern int
mtra_init (int * myRwnd);

struct alloc_t
{
    void* ptr;
    size_t allocsize;
};

TEST(test_case_logging, test_read_trace_levels)
{
  read_trace_levels ();
}

static bool
action (timer_id_t& id, void*, void*)
{
  EVENTLOG(VERBOSE, "timer triggered\n");
  return NOT_RESET_TIMER_FROM_CB;
}

TEST(TIMER_MODULE, test_timer_mgr)
{
  timer_mgr tm;
  timer_id_t ret1 = tm.add_timer (TIMER_TYPE_INIT, 1000, action);
  timer_id_t ret3 = tm.add_timer (TIMER_TYPE_SACK, 15, action);
  timer_id_t ret2 = tm.add_timer (TIMER_TYPE_SACK, 1, action);
  tm.print (VERBOSE);

  mysleep (20);
  EVENTLOG1(VERBOSE, "timeouts %d", tm.timeouts ());
  tm.delete_timer (ret3);
  tm.delete_timer (ret2);
  tm.delete_timer (ret1);
  EVENTLOG1(VERBOSE, "timeouts %d", tm.timeouts ());

  tm.print (VERBOSE);
}
TEST(TIMER_MODULE, test_operations_on_time)
{
  timeval tv;
  fills_timeval(&tv, 1000);
  EXPECT_TRUE(tv.tv_sec == 1);
  EXPECT_TRUE(tv.tv_usec == 0);

  timeval result;
  sum_time (&tv, (time_t) 200, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 1);
  EXPECT_TRUE(result.tv_usec == 200000);

  sum_time (&result, (time_t) 0, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 1);
  EXPECT_TRUE(result.tv_usec == 200000);

  sum_time (&result, (time_t) 1, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 1);
  EXPECT_TRUE(result.tv_usec == 201000);

  sum_time (&result, (time_t) 1000, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 2);
  EXPECT_TRUE(result.tv_usec == 201000);

  sum_time (&result, (time_t) 800, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 3);
  EXPECT_TRUE(result.tv_usec == 1000);

  subtract_time (&result, (time_t) 800, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 2);
  EXPECT_TRUE(result.tv_usec == 201000);

  subtract_time (&result, (time_t) 201, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 2);
  EXPECT_TRUE(result.tv_usec == 0);

  subtract_time (&result, (time_t) 0, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 2);
  EXPECT_TRUE(result.tv_usec == 0);

  subtract_time (&result, 2000, &result);
  //print_timeval(&result);
  EXPECT_TRUE(result.tv_sec == 0);
  EXPECT_TRUE(result.tv_usec == 0);
}
// last run on 21 Agu 2016 and passed
TEST(GLOBAL_MODULE, test_saddr_str)
{
  sockaddrunion saddr;
  str2saddr (&saddr, "192.168.1.107", 38000);

  char ret[MAX_IPADDR_STR_LEN];
  ushort port = 0;
  saddr2str (&saddr, ret, sizeof(ret), &port);
  EVENTLOG1(VERBOSE, "saddr {%s}\n", ret);
  EXPECT_EQ(saddr.sa.sa_family, AF_INET);
  EXPECT_EQ(strcmp (ret, "192.168.1.107"), 0);
  EXPECT_EQ(port, 38000);

  sockaddrunion saddr1;
  str2saddr (&saddr1, "192.168.1.107", 38000);
  sockaddrunion saddr2;
  str2saddr (&saddr2, "192.168.1.107", 38000);
  EXPECT_EQ(saddr_equals (&saddr1, &saddr2), true);

  str2saddr (&saddr1, "192.167.1.125", 38000);
  str2saddr (&saddr2, "192.168.1.107", 38000);
  EXPECT_EQ(saddr_equals (&saddr1, &saddr2), false);

  str2saddr (&saddr1, "192.168.1.107", 3800);
  str2saddr (&saddr2, "192.168.1.107", 38000);
  EXPECT_EQ(saddr_equals (&saddr1, &saddr2), false);

  str2saddr (&saddr1, "192.168.1.125", 3800);
  str2saddr (&saddr2, "192.168.1.107", 38000);
  EXPECT_EQ(saddr_equals (&saddr1, &saddr2), false);
}

// last run on 27 Agu 2016 and passed
TEST(MALLOC_MODULE, test_geco_new_delete)
{
  int j;
  int total = 1000000;
  /*max is 5120 we use 5121 to have the max*/
  size_t allocsize;
  size_t dealloc_idx;
  std::list<alloc_t*> allos;
  std::list<alloc_t*>::iterator it;

  int alloccnt = 0;
  int deallcnt = 0;
  alloc_t* at;
  for (j = 0; j < total; j++)
  {
    if (rand () % 2)
    {

      uint s = ((rand () * UINT32_MAX) % 1024) + 1;
      at = geco_new_array<alloc_t> (s, __FILE__, __LINE__);
      at->allocsize = s;
      allos.push_back (at);
      alloccnt += s;
    }
    else
    {
      size_t s = allos.size ();
      if (s > 0)
      {
        dealloc_idx = (rand () % s);
        it = allos.begin ();
        std::advance (it, dealloc_idx);
        deallcnt += (*it)->allocsize;
        geco_delete_array<alloc_t> (*it, __FILE__, __LINE__);
        allos.erase (it);
      }
    }
  }
  for (auto& p : allos)
  {
    deallcnt += p->allocsize;
    geco_delete_array<alloc_t> (p, __FILE__, __LINE__);
  }
  allos.clear ();
  EXPECT_EQ(alloccnt, deallcnt);
  EXPECT_EQ(allos.size (), 0);
}
TEST(MALLOC_MODULE, test_geco_alloc_dealloc)
{
  int j;
  int total = 1000000;
  /*max is 5120 we use 5121 to have the max*/
  size_t allocsize;
  size_t dealloc_idx;
  std::list<alloc_t> allos;
  std::list<alloc_t>::iterator it;

  int alloccnt = 0;
  int deallcnt = 0;
  int less_than_max_byte_cnt = 0;
  int zero_alloc_cnt = 0;
  alloc_t at;
  for (j = 0; j < total; j++)
  {
    if (rand () % 2)
    {
      allocsize = (rand () * UINT32_MAX) % 2049;
      if (allocsize <= 1512)
        ++less_than_max_byte_cnt;
      if (allocsize == 0)
        ++zero_alloc_cnt;
      at.ptr = geco_malloc_ext (allocsize, __FILE__, __LINE__);
      at.allocsize = allocsize;
      allos.push_back (at);
      alloccnt++;
    }
    else
    {
      size_t s = allos.size ();
      if (s > 0)
      {
        dealloc_idx = rand () % s;
        it = allos.begin ();
        std::advance (it, dealloc_idx);
        geco_free_ext (it->ptr, __FILE__, __LINE__);
        allos.erase (it);
        deallcnt++;
      }
    }
  }
  for (auto& p : allos)
  {
    geco_free_ext (p.ptr, __FILE__, __LINE__);
    deallcnt++;
  }
  allos.clear ();
  EXPECT_EQ(alloccnt, deallcnt);
  EXPECT_EQ(allos.size (), 0);
  EVENTLOG5(VERBOSE,
            "alloccnt %d, dealloccnt %d, < 1512 cnt %d, %d, zer alloc cnt %d\n",
            alloccnt, deallcnt, less_than_max_byte_cnt,
            alloccnt - less_than_max_byte_cnt, zero_alloc_cnt);
}
// last run on 21 Agu 2016 and passed
TEST(MALLOC_MODULE, test_alloc_dealloc)
{
  single_client_alloc allocator;
  int j;
  int total = 1000000;
  /*max is 5120 we use 5121 to have the max*/
  size_t allocsize;
  size_t dealloc_idx;
  std::list<alloc_t> allos;
  std::list<alloc_t>::iterator it;

  int alloccnt = 0;
  int deallcnt = 0;
  int less_than_max_byte_cnt = 0;
  int zero_alloc_cnt = 0;
  alloc_t at;
  for (j = 0; j < total; j++)
  {
    if (rand () % 2)
    {
      allocsize = (rand () * UINT32_MAX) % 2049;
      if (allocsize <= 1512)
        ++less_than_max_byte_cnt;
      if (allocsize == 0)
        ++zero_alloc_cnt;
      at.ptr = allocator.allocate (allocsize);
      at.allocsize = allocsize;
      allos.push_back (at);
      alloccnt++;
    }
    else
    {
      size_t s = allos.size ();
      if (s > 0)
      {
        dealloc_idx = rand () % s;
        it = allos.begin ();
        std::advance (it, dealloc_idx);
        allocator.deallocate (it->ptr, it->allocsize);
        allos.erase (it);
        deallcnt++;
      }
    }
  }
  for (auto& p : allos)
  {
    allocator.deallocate (p.ptr, p.allocsize);
    deallcnt++;
  }
  allos.clear ();
  allocator.destroy ();
  EXPECT_EQ(alloccnt, deallcnt);
  EXPECT_EQ(allos.size (), 0);
  EVENTLOG5(VERBOSE,
            "alloccnt %d, dealloccnt %d, < 1512 cnt %d, %d, zer alloc cnt %d\n",
            alloccnt, deallcnt, less_than_max_byte_cnt,
            alloccnt - less_than_max_byte_cnt, zero_alloc_cnt);
}

// last pass on 26 Oct 2016
TEST(AUTH_MODULE, test_md5)
{
  unsigned char digest[HMAC_LEN];
  MD5_CTX ctx;

  const char* testdata = "202cb962ac59075b964b07152d234b70";
  const char* result = "d9b1d7db4cd6e70935368a1efb10e377";
  MD5Init (&ctx);
  MD5Update (&ctx, (uchar*) testdata, strlen (testdata));
  MD5Final (digest, &ctx);
  EVENTLOG1(VERBOSE, "Computed MD5 signature : %s",
            hexdigest(digest, HMAC_LEN));
  EXPECT_STREQ(hexdigest (digest, 16), result);

  testdata = "d9b1d7db4cd6e70935368a1efb10e377";
  result = "7363a0d0604902af7b70b271a0b96480";
  MD5Init (&ctx);
  MD5Update (&ctx, (uchar*) testdata, strlen (testdata));
  MD5Final (digest, &ctx);
  EVENTLOG1(VERBOSE, "Computed MD5 signature : %s",
            hexdigest(digest, HMAC_LEN));
  EXPECT_STREQ(hexdigest (digest, 16), result);
}
TEST(AUTH_MODULE, test_sockaddr2hashcode)
{
  uint ret;
  sockaddrunion localsu;
  str2saddr (&localsu, "192.168.1.107", 36000);
  sockaddrunion peersu;
  str2saddr (&peersu, "192.168.1.107", 36000);
  ret = transportaddr2hashcode (&localsu, &peersu);
  EVENTLOG2(
      VERBOSE,
      "hash(addr pair { localsu: 192.168.1.107:36001 peersu: 192.168.1.107:36000 }) = %u, %u",
      ret, ret % 100000);

  str2saddr (&localsu, "192.168.1.107", 1234);
  str2saddr (&peersu, "192.168.1.107", 360);
  ret = transportaddr2hashcode (&localsu, &peersu);
  EVENTLOG2(
      VERBOSE,
      "hash(addr pair { localsu: 192.168.1.107:36001 peersu: 192.168.1.107:36000 }) = %u, %u",
      ret, ret % 100000);
}
TEST(AUTH_MODULE, test_crc32_checksum)
{
  for (int ii = 0; ii < 100; ii++)
  {
    geco_packet_t geco_packet;
    geco_packet.pk_comm_hdr.checksum = 0;
    geco_packet.pk_comm_hdr.dest_port = htons (
        (generate_random_uint32 () % USHRT_MAX));
    geco_packet.pk_comm_hdr.src_port = htons (
        (generate_random_uint32 () % USHRT_MAX));
    geco_packet.pk_comm_hdr.verification_tag = htons (
        (generate_random_uint32 ()));
    ((chunk_fixed_t*) geco_packet.chunk)->chunk_id = CHUNK_DATA;
    ((chunk_fixed_t*) geco_packet.chunk)->chunk_length = htons (100);
    ((chunk_fixed_t*) geco_packet.chunk)->chunk_flags =
    DCHUNK_FLAG_UNORDER | DCHUNK_FLAG_FL_FRG;
    for (int i = 0; i < 100; i++)
    {
      uchar* wt = geco_packet.chunk + CHUNK_FIXED_SIZE;
      wt[i] = generate_random_uint32 () % UCHAR_MAX;
    }
    set_crc32_checksum ((char*) &geco_packet, DATA_CHUNK_FIXED_SIZES + 100);
    bool ret = validate_crc32_checksum ((char*) &geco_packet,
    DATA_CHUNK_FIXED_SIZES + 100);
    EXPECT_TRUE(ret);
  }
}

static bool flag = true;
static timer_id_t tid;
static char inputs[1024];
static int len;
static void
stdin_cb (char* data, size_t datalen)
{
  EVENTLOG2(DEBUG, "stdin_cb()::%d bytes : %s", datalen, inputs);

  memcpy (inputs, data, datalen);
  if (strcmp (data, "q") == 0)
  {
    flag = false;
    return;
  }

  mtra_timer_mgr_.reset_timer (tid, 1000000);

  int sentsize;
  uchar tos = IPTOS_DEFAULT;
  sockaddrunion saddr;

  str2saddr (&saddr, "::1", USED_UDP_PORT);
  sentsize = mtra_send_rawsocks (mtra_read_ip6rawsock (), inputs, datalen,
                                 &saddr, tos);
  assert(sentsize == datalen);
  EXPECT_STRCASEEQ(data, inputs);

  str2saddr (&saddr, "127.0.0.1", USED_UDP_PORT);
  sentsize = mtra_send_rawsocks (mtra_read_ip4rawsock (), inputs, datalen,
                                 &saddr, tos);
  assert(sentsize == datalen);
  EXPECT_STRCASEEQ(data, inputs);

  str2saddr (&saddr, "::1", USED_UDP_PORT);
  sentsize = mtra_send_udpscoks (mtra_read_ip6udpsock (), inputs, datalen,
                                 &saddr, tos);
  assert(sentsize == datalen);
  EXPECT_STRCASEEQ(data, inputs);

  str2saddr (&saddr, "127.0.0.1", USED_UDP_PORT);
  sentsize = mtra_send_udpscoks (mtra_read_ip4udpsock (), inputs, datalen,
                                 &saddr, tos);
  assert(sentsize == datalen);
  EXPECT_STRCASEEQ(data, inputs);
}
static void
socket_cb (int sfd, char* data, int datalen, sockaddrunion* from,
           sockaddrunion* to)
{
  EXPECT_STRCASEEQ(data, inputs);

  static char fromstr[MAX_IPADDR_STR_LEN];
  static char tostr[MAX_IPADDR_STR_LEN];
  ushort fport;
  ushort tport;
  saddr2str (from, fromstr, MAX_IPADDR_STR_LEN, &fport);
  saddr2str (to, tostr, MAX_IPADDR_STR_LEN, &tport);

  if (sfd == mtra_read_ip4rawsock ())
  {
    EVENTLOG8(
        DEBUG,
        "socket_cb(ip%d raw fd=%d)::receive (%d) bytes  of data(%s) from addr (%s:%d) to addr (%s:%d)",
        4, sfd, datalen, data, fromstr, fport, tostr, tport);
  }
  else if (sfd == mtra_read_ip6rawsock ())
  {
    EVENTLOG8(
        DEBUG,
        "socket_cb(ip%d raw fd=%d)::receive (%d) bytes  of data(%s) from addr (%s:%d) to addr (%s:%d)",
        6, sfd, datalen, data, fromstr, fport, tostr, tport);
  }
  else if (sfd == mtra_read_ip6udpsock ())
  {
    EVENTLOG8(
        DEBUG,
        "socket_cb(ip%d udp fd=%d)::receive (%d) bytes  of data(%s) from addr (%s:%d) to addr (%s:%d)",
        6, sfd, datalen, data, fromstr, fport, tostr, tport);
  }
  else if (sfd == mtra_read_ip4udpsock ())
  {
    EVENTLOG8(
        DEBUG,
        "socket_cb(ip%d udp fd=%d)::receive (%d) bytes  of data(%s) from addr (%s:%d) to addr (%s:%d)",
        4, sfd, datalen, data, fromstr, fport, tostr, tport);
  }
  else
  {
    EVENTLOG(DEBUG, "NO SUCH SFD");
  }
}
static bool
timer_cb (timer_id_t& tid, void* a1, void* a2)
{
  EVENTLOG2(DEBUG, "timeouts, BYE!", tid->timer_id, tid->timer_type);
  flag = false;
  return true;
}
static void
task_cb (void* userdata)
{
  static int counter = 0;
  counter++;
  if (counter > 300)
  {
    EVENTLOG1(DEBUG,
              "task_cb called 300 times with tick of 10ms(userdata = %s)",
              (char* )userdata);
    counter = 0;
  }
}
TEST(TRANSPORT_MODULE, test_process_stdin)
{
  int rcwnd = 512;
  mtra_init (&rcwnd);

  cbunion_t cbunion;
  cbunion.socket_cb_fun = socket_cb;
  mtra_set_expected_event_on_fd (mtra_read_ip6rawsock (),
  EVENTCB_TYPE_SCTP,
                                 POLLIN | POLLPRI, cbunion, 0);
  mtra_set_expected_event_on_fd (mtra_read_ip6udpsock (),
  EVENTCB_TYPE_UDP,
                                 POLLIN | POLLPRI, cbunion, 0);
  mtra_set_expected_event_on_fd (mtra_read_ip4rawsock (),
  EVENTCB_TYPE_SCTP,
                                 POLLIN | POLLPRI, cbunion, 0);
  mtra_set_expected_event_on_fd (mtra_read_ip4udpsock (),
  EVENTCB_TYPE_UDP,
                                 POLLIN | POLLPRI, cbunion, 0);

  //you have to put stdin as last because we test it
  mtra_add_stdin_cb (stdin_cb);
  mtra_set_tick_task_cb (task_cb, (void*) "this is user datta");
  tid = mtra_timer_mgr_.add_timer (TIMER_TYPE_INIT, 300000, timer_cb, 0, 0);

  while (flag)
  {
    mtra_poll (0, 0, 0);
  }
  mtra_destroy ();
}
