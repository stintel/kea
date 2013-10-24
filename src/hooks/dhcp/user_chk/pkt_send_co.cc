// Copyright (C) 2013 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

/// @file Defines the pkt4_send and pkt6_send callout functions.

#include <asiolink/io_address.h>
#include <hooks/hooks.h>
#include <dhcp/pkt4.h>
#include <dhcp/dhcp6.h>
#include <dhcp/pkt6.h>
#include <user_chk.h>

using namespace isc::dhcp;
using namespace isc::hooks;
using namespace std;

// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.
extern "C" {

/// @brief Adds an entry to the end of the user check outcome file.
///
/// Each user entry is written in an ini-like format, with one name-value pair
/// per line as follows:
///
/// id_type=<id type>
/// client=<id str>
/// subnet=<subnet str>
/// registered=<is registered>"
///
/// where:
/// <id type> text label of the id type: "HW_ADDR" or "DUID"
/// <id str> user's id formatted as either isc::dhcp::Hwaddr.toText() or
/// isc::dhcp::DUID.toText()
/// <subnet str> selected subnet formatted as isc::dhcp::Subnet4::toText() or
/// isc::dhcp::Subnet6::toText() as appropriate.
/// <is registered> "yes" or "no"
///
/// Sample IPv4 entry would like this:
///
/// @code
/// id_type=DUID
/// client=00:01:00:01:19:ef:e6:3b:00:0c:01:02:03:04
/// subnet=2001:db8:2::/64
/// registered=yes
/// id_type=duid
/// @endcode
///
/// Sample IPv4 entry would like this:
///
/// @code
/// id_type=DUID
/// id_type=HW_ADDR
/// client=hwtype=1 00:0c:01:02:03:05
/// subnet=152.77.5.0/24
/// registered=no
/// @endcode
///
/// @param id_type_str text label identify the id type
/// @param id_val_str text representation of the user id
/// @param subnet_str text representation  of the selected subnet
/// @param registered boolean indicating if the user is registered or not
void generate_output_record(const std::string& id_type_str,
                            const std::string& id_val_str,
                            const std::string& addr_str,
                            const bool& registered)
{
    user_chk_output << "id_type=" << id_type_str << std::endl
                    << "client=" << id_val_str << std::endl
                    << "addr=" << addr_str << std::endl
                    << "registered=" << (registered ? "yes" : "no")
                    << std::endl;

    // @todo Flush is here to ensure output is immediate for demo purposes.
    // Performance would generally dictate not using it.
    flush(user_chk_output);
}


/// @brief  This callout is called at the "pkt4_send" hook.
///
/// This function searches the UserRegistry for the client indicated by the
/// inbound IPv4 DHCP packet. If the client is found  @todo
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int pkt4_send(CalloutHandle& handle) {
    try {
        Pkt4Ptr response;
        handle.getArgument("response4", response);

        // Get the user id saved from the query packet.
        HWAddrPtr hwaddr;
        handle.setContext(query_user_id_label, hwaddr);

        // Get registered_user pointer.
        UserPtr registered_user;
        handle.getContext(registered_user_label, registered_user);

        // Fetch the lease address.
        isc::asiolink::IOAddress addr = response->getYiaddr();

        if (registered_user) {
            // add options based on user
            // then generate registered output record
            std::cout << "DHCP UserCheckHook : pkt4_send registered_user is: "
                      << registered_user->getUserId() << std::endl;

            // Add the outcome entry to the output file.
            generate_output_record(UserId::HW_ADDRESS_STR, hwaddr->toText(),
                                   addr.toText(), true);
        } else {
            // add default options based 
            // then generate not registered output record
            std::cout << "DHCP UserCheckHook : pkt4_send no registered_user" 
                      << std::endl;
            // Add the outcome entry to the output file.
            generate_output_record(UserId::HW_ADDRESS_STR, hwaddr->toText(),
                                   addr.toText(), false);
        }
    } catch (const std::exception& ex) {
        std::cout << "DHCP UserCheckHook : pkt4_send unexpected error: "
                  << ex.what() << std::endl;
        return (1);
    }

    return (0);
}

/// @brief  This callout is called at the "pkt6_send" hook.
///
/// This function searches the UserRegistry for the client indicated by the
/// inbound IPv6 DHCP packet. If the client is found  @todo
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int pkt6_send(CalloutHandle& handle) {
    try {
        Pkt6Ptr response;
        handle.getArgument("response6", response);

        // Fetch the lease address. @todo
        isc::asiolink::IOAddress addr("0.0.0.0");

        // Get the user id saved from the query packet.
        DuidPtr duid;
        handle.setContext(query_user_id_label, duid);

        // Get registered_user pointer.
        UserPtr registered_user;
        handle.getContext(registered_user_label, registered_user);

        if (registered_user) {
            // add options based on user
            // then generate registered output record
            std::cout << "DHCP UserCheckHook : pkt6_send registered_user is: "
                      << registered_user->getUserId() << std::endl;
            // Add the outcome entry to the output file.
            generate_output_record(UserId::DUID_STR, duid->toText(),
                                   addr.toText(), true);
        } else {
            // add default options based 
            // then generate not registered output record
            std::cout << "DHCP UserCheckHook : pkt6_send no registered_user" 
                      << std::endl;
            // Add the outcome entry to the output file.
            generate_output_record(UserId::DUID_STR, duid->toText(),
                                   addr.toText(), false);
        }
    } catch (const std::exception& ex) {
        std::cout << "DHCP UserCheckHook : pkt6_send unexpected error: "
                  << ex.what() << std::endl;
        return (1);
    }

    return (0);
}

} // end extern "C"
