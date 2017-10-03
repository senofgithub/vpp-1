/*
 * Copyright (c) 2017 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef __VOM_BRIDGE_DOMAIN_H__
#define __VOM_BRIDGE_DOMAIN_H__

#include <string>
#include <stdint.h>

#include "vom/object_base.hpp"
#include "vom/om.hpp"
#include "vom/hw.hpp"
#include "vom/rpc_cmd.hpp"
#include "vom/dump_cmd.hpp"
#include "vom/singular_db.hpp"
#include "vom/enum_base.hpp"
#include "vom/inspect.hpp"

#include <vapi/l2.api.vapi.hpp>

namespace VOM
{
    /**
     * A base class for all object_base in the VPP object_base-Model.
     *  provides the abstract interface.
     */
    class bridge_domain: public object_base
    {
    public:
        /**
         * The value of the defaultbridge domain
         */
        const static uint32_t DEFAULT_TABLE = 0;

        /**
         * Construct a new object matching the desried state
         */
        bridge_domain(uint32_t id);
        /**
         * Copy Constructor
         */
        bridge_domain(const bridge_domain& o);
        /**
         * Destructor
         */
        ~bridge_domain();

        /**
         * Return the matchin 'singular' instance of the bridge-domain
         */
        std::shared_ptr<bridge_domain> singular() const;

        /**
         * convert to string format for debug purposes
         */
        std::string to_string(void) const;

        /**
         * Return VPP's handle for this obejct
         */
        uint32_t id() const;

        /**
         * Static function to find the bridge_domain in the model
         */
        static std::shared_ptr<bridge_domain> find(uint32_t id);

        /**
         * Dump all bridge-doamin into the stream provided
         */
        static void dump(std::ostream &os);

        /**
         * A command class that creates an Bridge-Domain
         */
        class create_cmd: public rpc_cmd<HW::item<uint32_t>,
                                         rc_t,
                                         vapi::Bridge_domain_add_del>
        {
        public:
            /**
             * Constructor
             */
            create_cmd(HW::item<uint32_t> &item);

            /**
             * Issue the command to VPP/HW
             */
            rc_t issue(connection &con);
            /**
             * convert to string format for debug purposes
             */
            std::string to_string() const;

            /**
             * Comparison operator - only used for UT
             */
            bool operator==(const create_cmd&i) const;
        };

        /**
         * A cmd class that Delete an Bridge-Domain
         */
        class delete_cmd: public rpc_cmd<HW::item<uint32_t>,
                                         rc_t,
                                         vapi::Bridge_domain_add_del>
        {
        public:
            /**
             * Constructor
             */
            delete_cmd(HW::item<uint32_t> &item);

            /**
             * Issue the command to VPP/HW
             */
            rc_t issue(connection &con);
            /**
             * convert to string format for debug purposes
             */
            std::string to_string() const;

            /**
             * Comparison operator - only used for UT
             */
            bool operator==(const delete_cmd&i) const;
        };

        /**
         * A cmd class that Dumps all the IPv4 L3 configs
         */
        class dump_cmd: public VOM::dump_cmd<vapi::Bridge_domain_dump>
        {
        public:
            /**
             * Constructor
             */
            dump_cmd();
            dump_cmd(const dump_cmd &d);

            /**
             * Issue the command to VPP/HW
             */
            rc_t issue(connection &con);
            /**
             * convert to string format for debug purposes
             */
            std::string to_string() const;

            /**
             * Comparison operator - only used for UT
             */
            bool operator==(const dump_cmd&i) const;
        private:
            /**
             * HW reutrn code
             */
            HW::item<bool> item;
        };

    private:
        /**
         * Class definition for listeners to OM events
         */
        class event_handler: public OM::listener, public inspect::command_handler
        {
        public:
            event_handler();
            virtual ~event_handler() = default;

            /**
             * Handle a populate event
             */
            void handle_populate(const client_db::key_t & key);

            /**
             * Handle a replay event
             */
            void handle_replay();

            /**
             * Show the object in the Singular DB
             */
            void show(std::ostream &os);

            /**
             * Get the sortable Id of the listener
             */
            dependency_t order() const;
        };

        /**
         * Instance of the event handler to register with OM
         */
        static event_handler m_evh;

        /**
         * Commit the acculmulated changes into VPP. i.e. to a 'HW" write.
         */
        void update(const bridge_domain &obj);

        /**
         * Find or add an singular of a Bridge-Domain in the object_base Model
         */
        static std::shared_ptr<bridge_domain> find_or_add(const bridge_domain &temp);

        /*
         * It's the VOM::OM class that calls singular()
         */
        friend class VOM::OM;

        /**
         * It's the VOM::singular_db class that calls replay()
         */
        friend class VOM::singular_db<uint32_t, bridge_domain>;

        /**
         * Sweep/reap the object if still stale
         */
        void sweep(void);

        /**
         * replay the object to create it in hardware
         */
        void replay(void);

        /**
         * The ID we assign to this BD and the HW result in VPP
         */
        HW::item<uint32_t> m_id;

        /**
         * A map of all interfaces key against the interface's name
         */
        static singular_db<uint32_t, bridge_domain> m_db;
    };
};

#endif
