#ifndef WAPI_H
#define WAPI_H


#include <sys/socket.h>
#include <net/ethernet.h>
#include <linux/wireless.h>


/**
 * @mainpage Wireless API (WAPI)
 *
 * WAPI provides an easy-to-use function set to configure wireless network
 * interfaces on a GNU/Linux system. One can think WAPI as a lightweight C API
 * for @c iwconfig, @c wlanconfig and @c ifconfig commands. (But it is not a
 * thin wrapper for these command line tools.) It is designed to be used in
 * wireless heteregenous network research projects and supported by <a
 * href="http://bwrc.eecs.berkeley.edu/">BWRC (Berkeley Wireless Research
 * Center)</a> and <a href="http://www.wiserlab.org/">WISERLAB (Wireless
 * Information Systems Engineering Research Laboratory at Özyeğin
 * University)</a>.
 */


/**
 * @defgroup accessors Network Interface Accessors
 *
 * This section is composed of accessors (getters and setters) for various
 * network interface configurations. Each accessor requires a socket (see
 * wapi_make_socket()) to issue kernel commands. Since each accessor is a
 * user-friendly wrapper over various ioctl() calls, return values of the
 * accessors are generally obtained by related ioctl() calls. In other words, on
 * success, zero is returned.
 *
 * Pay attention that, all setters require root privileges.
 */


/**
 * @defgroup freq Frequency Accessors
 * @ingroup accessors
 * @{
 */


/** Frequency flags. */
typedef enum {
	WAPI_FREQ_AUTO	= IW_FREQ_AUTO,
	WAPI_FREQ_FIXED	= IW_FREQ_FIXED
} wapi_freq_flag_t;


/** Frequency flag names. */
extern const char *wapi_freq_flags[];


/**
 * Gets the operating frequency of the device.
 */
int wapi_get_freq(
	int sock,
	const char *ifname,
	double *freq,
	wapi_freq_flag_t *flag);


/**
 * Sets the operating frequency of the device.
 */
int
wapi_set_freq(
	int sock,
	const char *ifname,
	double freq,
	wapi_freq_flag_t flag);


/** @} */


/**
 * @defgroup essid ESSID (Extended Service Set Identifier) Accessors
 * @ingroup accessors
 * @{
 */


/** Maximum allowed ESSID size. */
#define WAPI_ESSID_MAX_SIZE IW_ESSID_MAX_SIZE


/** ESSID flags.  */
typedef enum {
	WAPI_ESSID_ON,
	WAPI_ESSID_OFF
} wapi_essid_flag_t;


/** ESSID flag names. */
extern const char *wapi_essid_flags[];


/**
 * Gets ESSID of the device.
 *
 * @param[out] essid Buffer will be used to store the ESSID of the device. Must
 *     of size @c WAPI_ESSID_MAX_SIZE+1.
 */
int
wapi_get_essid(
	int sock,
	const char *ifname,
	char *essid,
	wapi_essid_flag_t *flag);


/**
 * Sets ESSID of the device.
 *
 * @a essid At most @c WAPI_ESSID_MAX_SIZE characters are read.
 */
int
wapi_set_essid(
	int sock,
	const char *ifname,
	const char *essid,
	wapi_essid_flag_t flag);


/** @} */


/**
 * @defgroup mode Operating Mode
 * @ingroup accessors
 * @{
 */


/** Supported operation modes. */
typedef enum {
	WAPI_MODE_AUTO		= IW_MODE_AUTO,		/**< Driver decides. */
	WAPI_MODE_ADHOC		= IW_MODE_ADHOC,	/**< Single cell network. */
	WAPI_MODE_MANAGED	= IW_MODE_INFRA,	/**< Multi cell network, roaming, ... */
	WAPI_MODE_MASTER	= IW_MODE_MASTER,	/**< Synchronisation master or access point. */
	WAPI_MODE_REPEAT	= IW_MODE_REPEAT,	/**< Wireless repeater, forwarder. */
	WAPI_MODE_SECOND	= IW_MODE_SECOND,	/**< Secondary master/repeater, backup. */
	WAPI_MODE_MONITOR	= IW_MODE_MONITOR	/**< Passive monitor, listen only. */
} wapi_mode_t;


/** Supported operation mode names. */
extern const char *wapi_modes[];


/**
 * Gets the operating mode of the device.
 */
int wapi_get_mode(int sock, const char *ifname, wapi_mode_t *mode);


/**
 * Sets the operating mode of the device.
 */
int wapi_set_mode(int sock, const char *ifname, wapi_mode_t mode);


/** @} */


/**
 * @defgroup ap Access Point
 * @ingroup accessors
 * @{
 */


/**
 * Creates an ethernet broadcast address.
 */
void wapi_make_broad_ether(struct sockaddr *sa);


/**
 * Creates an ethernet NULL address.
 */
void wapi_make_null_ether(struct sockaddr *sa);


/**
 * Gets access point address of the device. For "any", a broadcast ethernet
 * address; for "off", a null ethernet address is returned. Returned @c struct
 * sockaddr is of @c ARPHRD_ETHER family.
 */
int wapi_get_ap(int sock, const char *ifname, struct sockaddr *ap);


/**
 * Sets access point address of the device. Supplied socket address must be of
 * @c ARPHRD_ETHER family.
 */
int wapi_set_ap(int sock, const char *ifname, const struct sockaddr *ap);


/** @} */


/**
 * @defgroup bitrate Bit Rate
 * @ingroup accessors
 * @{
 */


/**
 * Bitrate flags.
 *
 * At the moment, unicast (@c IW_BITRATE_UNICAST) and broadcast (@c
 * IW_BITRATE_BROADCAST) bitrate flags are not supported.
 */
typedef enum {
	WAPI_BITRATE_AUTO,
	WAPI_BITRATE_FIXED
} wapi_bitrate_flag_t;


/** Bitrate flag names. */
extern const char *wapi_bitrate_flags[];


/**
 * Gets bitrate of the device.
 */
int wapi_get_bitrate(
	int sock,
	const char *ifname,
	int *bitrate,
	wapi_bitrate_flag_t *flag);


/**
 * Sets bitrate of the device.
 */
int
wapi_set_bitrate(
	int sock,
	const char *ifname,
	int bitrate,
	wapi_bitrate_flag_t flag);


/** @} */


/**
 * @defgroup txpower Transmit Power
 * @ingroup accessors
 * @{
 */


/** Transmit power (txpower) flags. */
typedef enum {
	WAPI_TXPOWER_DBM,		/**< Value is in dBm. */
	WAPI_TXPOWER_MWATT,		/**< Value is in mW. */
	WAPI_TXPOWER_RELATIVE	/**< Value is in arbitrary units. */
} wapi_txpower_flag_t;


/** Transmit power flag names. */
extern const char *wapi_txpower_flags[];


/**
 * Converts a value in dBm to a value in milliWatt.
 */
int wapi_dbm2mwatt(int dbm);


/**
 * Converts a value in milliWatt to a value in dBm.
 */
int wapi_mwatt2dbm(int mwatt);


/**
 * Gets txpower of the device.
 */
int
wapi_get_txpower(
	int sock,
	const char *ifname,
	int *power,
	wapi_txpower_flag_t *flag);


/**
 * Sets txpower of the device.
 */
int
wapi_set_txpower(
	int sock,
	const char *ifname,
	int power,
	wapi_txpower_flag_t flag);


/** @} */


/**
 * @defgroup commonstructs Common Data Structures
 * @{
 */


/** Linked list container for strings. */
typedef struct wapi_string_t {
	struct wapi_string_t *next;
	char *data;
} wapi_string_t;


/** Linked list container for scan results. */
typedef struct wapi_scan_info_t {
	struct wapi_scan_info_t *next;
	struct sockaddr *ap;
	int has_essid;
	char essid[WAPI_ESSID_MAX_SIZE+1];
	wapi_essid_flag_t essid_flag;
	int has_freq;
	double freq;
	int has_mode;
	wapi_mode_t mode;
	int has_bitrate;
	int bitrate;
} wapi_scan_info_t;


/**
 * A generic linked list container. For functions taking @c wapi_list_t type of
 * argument, caller is resposible for releasing allocated memory.
 */
typedef struct wapi_list_t {
	union wapi_list_head_t {
		wapi_string_t *string;
		wapi_scan_info_t *scan;
	} head;
} wapi_list_t;


/** @} */


/**
 * @defgroup utils Utility Routines
 * @{
 */


/**
 * Creates an AF_INET socket to be used in ioctl() calls.
 *
 * @return non-negative if succeeds.
 */
int wapi_make_socket(void);


/**
 * Gets kernel WE (Wireless Extensions) version.
 *
 * @param[out] we_version Set to @c we_version_compiled of range information.
 *
 * @return zero if succeeds.
 */
int wapi_get_we_version(int sock, const char *ifname, int *we_version);


/** Path to @c /proc/net/wireless. (Requires procfs mounted.) */
#define WAPI_PROC_NET_WIRELESS "/proc/net/wireless"


/**
 * Parses @c WAPI_PROC_NET_WIRELESS according to hardcoded mechanisms in @c
 * linux/net/wireless/wext-proc.c sources.
 *
 * @return @c 0, on success.
 */
int wapi_get_ifnames(wapi_list_t *list);


/** @} */


/**
 * @defgroup scan Scanning
 *
 * This group consists of functions for scanning accessible access points (APs)
 * in the range.
 *
 * Unfortunately provided scanning API by wireless-tools libraries (libiw) is
 * quite limited, and doesn't list all APs in the range. (See iw_process_scan()
 * of libiw. For iwlist case, it has its own hardcoded magic for this stuff and
 * it is not provided by libiw.) For this purpose, we needed to implement our
 * own scanning routines. Furthermore, scanning requires extracting binary
 * results returned from kernel over a @c char buffer, hence it causes dozens of
 * hairy binary compatibility issues. Luckily, libiw provide an API method to
 * cope with this: iw_extract_event_stream(). That's the only place in this
 * project relying on libiw.
 *
 * The scanning operation disable normal network traffic, and therefore you
 * should not abuse of scan. The scan need to check the presence of network on
 * other frequencies. While you are checking those other frequencies, you can
 * not be on your normal frequency to listen to normal traffic in the cell. You
 * need typically in the order of one second to actively probe all 802.11b
 * channels (do the maths). Some cards may do that in background, to reply to
 * scan commands faster, but they still have to do it. Leaving the cell for such
 * an extended period of time is pretty bad. Any kind of streaming/low latency
 * traffic will be impacted, and the user will perceive it (easily checked with
 * telnet). People trying to send traffic to you will retry packets and waste
 * bandwidth. Some applications may be sensitive to those packet losses in weird
 * ways, and tracing those weird behavior back to scanning may take time. If you
 * are in ad-hoc mode, if two nodes scan approx at the same time, they won't see
 * each other, which may create associations issues. For those reasons, the
 * scanning activity should be limited to what's really needed, and continuous
 * scanning is a bad idea. --- Jean Tourrilhes
 *
 * @{
 */


/**
 * Starts a scan on the given interface. Root privileges are required to start a
 * scan.
 */
int wapi_scan_init(int sock, const char *ifname);


/**
 * Checks the status of the scan process.
 *
 * @return zero, if data is ready; 1, if data is not ready; negative on failure.
 */
int wapi_scan_stat(int sock, const char *ifname);


/**
 * Collects the results of a scan process.
 *
 * @param[out] aps Pushes collected @c wapi_scan_info_t into this list.
 */
int wapi_scan_coll(int sock, const char *ifname, wapi_list_t *aps);


/** @} */


#endif /* WAPI_H */