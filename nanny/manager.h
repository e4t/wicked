/*
 * Declarations to the manager service. This service is supposed
 * to bring up interfaces in response to events, such as link detection
 * or presence of wireless networks.
 *
 * Copyright (C) 2010-2012 Olaf Kirch <okir@suse.de>
 */


#ifndef __WICKED_MANAGER_H__
#define __WICKED_MANAGER_H__

#include <wicked/fsm.h>

typedef struct ni_manager	ni_manager_t;
typedef struct ni_managed_netdev ni_managed_netdev_t;
typedef struct ni_managed_policy ni_managed_policy_t;
typedef struct ni_managed_modem	ni_managed_modem_t;
typedef struct ni_manager_secret ni_manager_secret_t;

struct ni_managed_netdev {
	ni_managed_netdev_t *	next;

	ni_manager_t *		manager;	// back pointer at mgr
	ni_netdev_t *		dev;
	ni_ifworker_t *		worker;
	ni_bool_t		user_controlled;

	ni_managed_policy_t *	selected_policy;
	unsigned int		selected_policy_seq;
	xml_node_t *		selected_config;
	unsigned long		timeout;
};

struct ni_managed_modem {
	ni_managed_modem_t *	next;

	ni_manager_t *		manager;	// back pointer at mgr
	ni_modem_t *		dev;
	ni_ifworker_t *		worker;
	ni_bool_t		user_controlled;

	ni_managed_policy_t *	selected_policy;
	unsigned int		selected_policy_seq;
	xml_node_t *		selected_config;
	unsigned long		timeout;
};

struct ni_managed_policy {
	ni_managed_policy_t *	next;

	unsigned int		seqno;
	ni_fsm_policy_t *	fsm_policy;
	xml_document_t *	doc;
};

struct ni_manager {
	ni_dbus_server_t *	server;
	ni_fsm_t *		fsm;

	ni_managed_netdev_t *	netdev_list;
	ni_managed_modem_t *	modem_list;
	ni_managed_policy_t *	policy_list;

	ni_ifworker_array_t	recheck;
	ni_ifworker_array_t	down;

	unsigned int		policy_seq;

	ni_manager_secret_t *	secret_db;
};

extern ni_dbus_class_t		managed_netdev_class;
extern ni_dbus_class_t		ni_objectmodel_managed_modem_class;
extern ni_dbus_class_t		managed_policy_class;
extern ni_dbus_class_t		ni_objectmodel_manager_class;
extern ni_dbus_service_t	managed_netdev_service;
extern ni_dbus_service_t	ni_objectmodel_managed_modem_service;
extern ni_dbus_service_t	managed_policy_service;
extern ni_dbus_service_t	ni_objectmodel_manager_service;

extern ni_manager_t *		ni_manager_new(void);
extern void			ni_manager_free(ni_manager_t *);
extern void			ni_manager_schedule_recheck(ni_manager_t *, ni_ifworker_t *);
extern void			ni_manager_schedule_down(ni_manager_t *, ni_ifworker_t *);
extern void			ni_manager_recheck(ni_manager_t *, ni_ifworker_t *);
extern ni_managed_netdev_t *	ni_manager_get_netdev(ni_manager_t *, ni_netdev_t *);
extern ni_managed_modem_t *	ni_manager_get_modem(ni_manager_t *, ni_modem_t *);
extern ni_managed_policy_t *	ni_manager_get_policy(ni_manager_t *, const ni_fsm_policy_t *);
extern void			ni_manager_apply_policy(ni_manager_t *, ni_managed_policy_t *, ni_ifworker_t *);
extern const char *		ni_manager_get_secret(ni_manager_t *, const char *, const char *);

extern ni_managed_netdev_t *	ni_managed_netdev_new(ni_manager_t *, ni_ifworker_t *);
extern void			ni_managed_netdev_free(ni_managed_netdev_t *);
extern ni_bool_t		ni_managed_netdev_enable(ni_managed_netdev_t *);
extern void			ni_managed_netdev_apply_policy(ni_managed_netdev_t *, ni_managed_policy_t *, ni_fsm_t *);
extern void			ni_managed_netdev_up(ni_managed_netdev_t *, unsigned int);

extern ni_managed_modem_t *	ni_managed_modem_new(ni_manager_t *, ni_ifworker_t *);
extern void			ni_managed_modem_free(ni_managed_modem_t *);
extern void			ni_managed_modem_apply_policy(ni_managed_modem_t *, ni_managed_policy_t *, ni_fsm_t *);
extern void			ni_managed_modem_up(ni_managed_modem_t *, unsigned int);

extern ni_managed_policy_t *	ni_managed_policy_new(ni_manager_t *, ni_fsm_policy_t *, xml_document_t *);
extern void			ni_managed_policy_free(ni_managed_policy_t *);

extern ni_dbus_object_t *	ni_objectmodel_register_managed_netdev(ni_dbus_server_t *, ni_managed_netdev_t *);
extern ni_dbus_object_t *	ni_objectmodel_register_managed_modem(ni_dbus_server_t *, ni_managed_modem_t *);
extern ni_dbus_object_t *	ni_objectmodel_register_managed_policy(ni_dbus_server_t *, ni_managed_policy_t *);

extern void			interface_manager_register_all(ni_dbus_server_t *);
extern void			ni_objectmodel_manager_init(ni_manager_t *mgr);
extern void			ni_objectmodel_managed_netif_init(ni_dbus_server_t *);
extern void			ni_objectmodel_managed_modem_init(ni_dbus_server_t *);
extern void			ni_objectmodel_managed_policy_init(ni_dbus_server_t *);

#endif /* __WICKED_MANAGER_H__ */
