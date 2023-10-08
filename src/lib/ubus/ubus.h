#ifndef UBUS_H
#define UBUS_H

#include <exceptions/exceptions.h>

using namespace isc;
using namespace std;

namespace isc {

/// @brief Forward declaration to @ref Ubus.
class Ubus {
public:
	void init();
private:
	struct ubus_context *ctx;
};

/// @brief Exception thrown on failure to connect to ubus
class UbusConnectError : public Exception {
public:
	UbusConnectError(const char* file, size_t line, const char* what) :
		isc::Exception(file, line, what) {}
};

} // end of isc namespace

#endif // UBUS_H
