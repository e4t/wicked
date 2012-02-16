/*
 * Execute the requested process (almost) as if it were a
 * setuid process
 *
 * Copyright (C) 2002-2012 Olaf Kirch <okir@suse.de>
 */

#ifndef __WICKED_PROCESS_H__
#define __WICKED_PROCESS_H__

#include <wicked/logging.h>
#include <wicked/util.h>

/* FIXME: rename to ni_shellcmd */
struct ni_shellcmd {
	unsigned int		refcount;

	char *			command;
	ni_string_array_t	environ;

	unsigned int		timeout;
};

/* FIXME: rename to ni_process */
struct ni_process_instance {
	ni_shellcmd_t *		process;

	pid_t			pid;
	int			status;
	ni_socket_t *		socket;

	ni_string_array_t	argv;
	ni_string_array_t	environ;

	void			(*notify_callback)(ni_process_instance_t *);
	void *			user_data;
};

extern ni_shellcmd_t *		ni_shellcmd_new(const char *);
extern ni_process_instance_t *	ni_process_instance_new(ni_shellcmd_t *);
extern int			ni_process_instance_run(ni_process_instance_t *);
extern int			ni_process_instance_run_and_wait(ni_process_instance_t *);
extern void			ni_process_instance_setenv(ni_process_instance_t *, const char *, const char *);
extern const char *		ni_process_instance_getenv(const ni_process_instance_t *, const char *);
extern void			ni_process_instance_free(ni_process_instance_t *);
extern int			ni_process_exit_status_okay(const ni_process_instance_t *);
extern void			ni_shellcmd_free(ni_shellcmd_t *);

static inline ni_shellcmd_t *
ni_shellcmd_hold(ni_shellcmd_t *proc)
{
	ni_assert(proc->refcount);
	proc->refcount++;
	return proc;
}

static inline void
ni_shellcmd_release(ni_shellcmd_t *proc)
{
	ni_assert(proc->refcount);
	if (--(proc->refcount) == 0)
		ni_shellcmd_free(proc);
}

#endif /* __WICKED_PROCESS_H__ */
