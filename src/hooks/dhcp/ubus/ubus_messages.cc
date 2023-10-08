// File created from ../../../../src/hooks/dhcp/ubus/ubus_messages.mes

#include <cstddef>
#include <log/message_types.h>
#include <log/message_initializer.h>

extern const isc::log::MessageID UBUS_LOAD = "UBUS_LOAD";
extern const isc::log::MessageID UBUS_UNLOAD = "UBUS_UNLOAD";

namespace {

const char* values[] = {
    "UBUS_LOAD", "Ubus hooks library has been loaded",
    "UBUS_UNLOAD", "Ubus hooks library has been unloaded",
    NULL
};

const isc::log::MessageInitializer initializer(values);

} // Anonymous namespace

