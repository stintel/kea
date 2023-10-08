#include <ubus.h>
#include <ubus_log.h>
#include <dhcpsrv/cfgmgr.h>
#include <hooks/hooks.h>
#include <process/daemon.h>

using namespace isc;
using namespace isc::dhcp;
using namespace isc::hooks;
using namespace isc::log;
using namespace isc::process;
using namespace isc::ubus;

// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.
extern "C" {

/// @brief This is a command callout for 'ubus-lease4-get' command.
///
/// @param handle Callout handle used to retrieve a command and
/// provide a response.
/// @return 0 if this callout has been invoked successfully,
/// 1 otherwise.
int ubus_lease4_get(CalloutHandle& handle) {
	LOG_INFO(ubus_logger, "ubus_lease4_get()");
	return(0);
}

/// @brief This is a command callout for 'ubus-lease6-get' command.
///
/// @param handle Callout handle used to retrieve a command and
/// provide a response.
/// @return 0 if this callout has been invoked successfully,
/// 1 otherwise.
int ubus_lease6_get(CalloutHandle& handle) {
	LOG_INFO(ubus_logger, "ubus_lease6_get()");
	return(0);
}

/// @brief This function is called when the library is loaded.
///
/// @param handle library handle
/// @return 0 when initialization is successful, 1 otherwise
int load(LibraryHandle& handle) {
    // Make the hook library not loadable by d2 or ca.
    uint16_t family = CfgMgr::instance().getFamily();
    const std::string& proc_name = Daemon::getProcName();
    if (family == AF_INET) {
        if (proc_name != "kea-dhcp4") {
            isc_throw(isc::Unexpected, "Bad process name: " << proc_name
                      << ", expected kea-dhcp4");
        }
    } else {
        if (proc_name != "kea-dhcp6") {
            isc_throw(isc::Unexpected, "Bad process name: " << proc_name
                      << ", expected kea-dhcp6");
        }
    }

    handle.registerCommandCallout("ubus-lease4-get", ubus_lease4_get);
    handle.registerCommandCallout("ubus-lease6-get", ubus_lease6_get);
    LOG_INFO(ubus_logger, UBUS_LOAD);
    return (0);
}

/// @brief This function is called when the library is unloaded.
///
/// @return 0 if deregistration was successful, 1 otherwise
int unload() {
    LOG_INFO(ubus_logger, UBUS_UNLOAD);
    return (0);
}

/// @brief This function is called to retrieve the multi-threading compatibility.
///
/// @return 1 which means compatible with multi-threading.
int multi_threading_compatible() {
    return (1);
}

} // end extern "C"
