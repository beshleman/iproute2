/* SPDX-License-Identifier: GPL-2.0 */
/*
 * iplink_vsock.c		VSOCK device support
 */

#include "utils.h"
#include "ip_common.h"

static void print_usage(FILE *f)
{
	fprintf(f, "Usage: ... vsock [ cid CID ]\n");
}

static int vsock_parse_opt(struct link_util *lu, int argc, char **argv,
			 struct nlmsghdr *n)
{
	int ret;

	while (argc > 0) {
		if (strcmp(*argv, "cid") == 0) {
			__u32 cid;

			NEXT_ARG();
			ret = get_u32(&cid, *argv, 10);
			if (ret < 0)
				invarg("invalid CID", *argv);
			addattr32(n, 1024, IFLA_VSOCK_CID, cid);
		} else if (strcmp(*argv, "help") == 0) {
			print_usage(stderr);
			return -1;
		} else {
			fprintf(stderr, "vsock: unknown command \"%s\"?\n", *argv);
			print_usage(stderr);
			return -1;
		}
		argc--;
		argv++;
	}

	return 0;
}

static void vsock_print_opt(struct link_util *lu, FILE *f, struct rtattr *tb[])
{
	char s2[16];

	if (!tb)
		return;

	if (tb[IFLA_VSOCK_CID]) {
		__u32 cid = rta_getattr_u32(tb[IFLA_VSOCK_CID]);

		snprintf(s2, sizeof(s2), "%u", cid);
		print_string(PRINT_ANY, "cid", "cid %s ", s2);
	}
}

static void vsock_print_help(struct link_util *lu, int argc, char **argv,
			   FILE *f)
{
	print_usage(f);
}

struct link_util vsock_link_util = {
	.id		= "vsock",
	.maxattr	= IFLA_VSOCK_MAX,
	.parse_opt	= vsock_parse_opt,
	.print_opt	= vsock_print_opt,
	.print_help     = vsock_print_help,
};
