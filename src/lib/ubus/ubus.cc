#include <iostream>
#include <string>
#include <libubus.h>
#include <ubus/ubus.h>

using namespace isc;
using namespace std;

namespace isc {

void
Ubus::init() {
	ctx = ubus_connect(NULL);
	if (!ctx) {
		isc_throw(UbusConnectError, "unable to connect to the ubus daemon");
	}


}

} // namespace isc
