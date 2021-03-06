#ifndef _IP_SET_H
#define _IP_SET_H

/* Copyright (C) 2000-2002 Joakim Axelsson <gozem@linux.nu>
 *                         Patrick Schaaf <bof@bof.de>
 *                         Martin Josefsson <gandalf@wlug.westbo.se>
 * Copyright (C) 2003-2004 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#if 0
#define IP_SET_DEBUG
#endif

/*
 * A sockopt of such quality has hardly ever been seen before on the open
 * market!  This little beauty, hardly ever used: above 64, so it's
 * traditionally used for firewalling, not touched (even once!) by the
 * 2.0, 2.2 and 2.4 kernels!
 *
 * Comes with its own certificate of authenticity, valid anywhere in the
 * Free world!
 *
 * Rusty, 19.4.2000
 */
#define SO_IP_SET 		83

/*
 * Heavily modify by Joakim Axelsson 08.03.2002
 * - Made it more modulebased
 *
 * Additional heavy modifications by Jozsef Kadlecsik 22.02.2004
 * - bindings added
 * - in order to "deal with" backward compatibility, renamed to ipset
 */

/*
 * Used so that the kernel module and ipset-binary can match their versions
 */
#define IP_SET_PROTOCOL_VERSION 2

#define IP_SET_MAXNAMELEN 32	/* set names and set typenames */

/* Lets work with our own typedef for representing an IP address.
 * We hope to make the code more portable, possibly to IPv6...
 *
 * The representation works in HOST byte order, because most set types
 * will perform arithmetic operations and compare operations.
 *
 * For now the type is an uint32_t.
 *
 * Make sure to ONLY use the functions when translating and parsing
 * in order to keep the host byte order and make it more portable:
 *  parse_ip()
 *  parse_mask()
 *  parse_ipandmask()
 *  ip_tostring()
 * (Joakim: where are they???)
 */

typedef uint32_t ip_set_ip_t;

/* Sets are identified by an id in kernel space. Tweak with ip_set_id_t
 * and IP_SET_INVALID_ID if you want to increase the max number of sets.
 */
typedef uint16_t ip_set_id_t;

#define IP_SET_INVALID_ID	65535

/* How deep we follow bindings */
#define IP_SET_MAX_BINDINGS	6

/*
 * Option flags for kernel operations (ipt_set_info)
 */
#define IPSET_SRC 		0x01	/* Source match/add */
#define IPSET_DST		0x02	/* Destination match/add */
#define IPSET_MATCH_INV		0x04	/* Inverse matching */

/*
 * Set features
 */
#define IPSET_TYPE_IP		0x01	/* IP address type of set */
#define IPSET_TYPE_PORT		0x02	/* Port type of set */
#define IPSET_DATA_SINGLE	0x04	/* Single data storage */
#define IPSET_DATA_DOUBLE	0x08	/* Double data storage */

/* Reserved keywords */
#define IPSET_TOKEN_DEFAULT	":default:"
#define IPSET_TOKEN_ALL		":all:"

/* SO_IP_SET operation constants, and their request struct types.
 *
 * Operation ids:
 *	  0-99:	 commands with version checking
 *	100-199: add/del/test/bind/unbind
 *	200-299: list, save, restore
 */

/* Single shot operations:
 * version, create, destroy, flush, rename and swap
 *
 * Sets are identified by name.
 */

#define IP_SET_REQ_STD		\
	unsigned op;		\
	unsigned version;	\
	char name[IP_SET_MAXNAMELEN]

#define IP_SET_OP_CREATE	0x00000001	/* Create a new (empty) set */
struct ip_set_req_create {
	IP_SET_REQ_STD;
	char typename[IP_SET_MAXNAMELEN];
};

#define IP_SET_OP_DESTROY	0x00000002	/* Remove a (empty) set */
struct ip_set_req_std {
	IP_SET_REQ_STD;
};

#define IP_SET_OP_FLUSH		0x00000003	/* Remove all IPs in a set */
/* Uses ip_set_req_std */

#define IP_SET_OP_RENAME	0x00000004	/* Rename a set */
/* Uses ip_set_req_create */

#define IP_SET_OP_SWAP		0x00000005	/* Swap two sets */
/* Uses ip_set_req_create */

union ip_set_name_index {
	char name[IP_SET_MAXNAMELEN];
	ip_set_id_t index;
};

#define IP_SET_OP_GET_BYNAME	0x00000006	/* Get set index by name */
struct ip_set_req_get_set {
	unsigned op;
	unsigned version;
	union ip_set_name_index set;
};

#define IP_SET_OP_GET_BYINDEX	0x00000007	/* Get set name by index */
/* Uses ip_set_req_get_set */

#define IP_SET_OP_VERSION	0x00000100	/* Ask kernel version */
struct ip_set_req_version {
	unsigned op;
	unsigned version;
};

/* Double shots operations:
 * add, del, test, bind and unbind.
 *
 * First we query the kernel to get the index and type of the target set,
 * then issue the command. Validity of IP is checked in kernel in order
 * to minimalize sockopt operations.
 */

/* Get minimal set data for add/del/test/bind/unbind IP */
#define IP_SET_OP_ADT_GET	0x00000010	/* Get set and type */
struct ip_set_req_adt_get {
	unsigned op;
	unsigned version;
	union ip_set_name_index set;
	char typename[IP_SET_MAXNAMELEN];
};

#define IP_SET_REQ_BYINDEX	\
	unsigned op;		\
	ip_set_id_t index;

struct ip_set_req_adt {
	IP_SET_REQ_BYINDEX;
};

#define IP_SET_OP_ADD_IP	0x00000101	/* Add an IP to a set */
/* Uses ip_set_req_adt, with type specific addage */

#define IP_SET_OP_DEL_IP	0x00000102	/* Remove an IP from a set */
/* Uses ip_set_req_adt, with type specific addage */

#define IP_SET_OP_TEST_IP	0x00000103	/* Test an IP in a set */
/* Uses ip_set_req_adt, with type specific addage */

#define IP_SET_OP_BIND_SET	0x00000104	/* Bind an IP to a set */
/* Uses ip_set_req_bind, with type specific addage */
struct ip_set_req_bind {
	IP_SET_REQ_BYINDEX;
	char binding[IP_SET_MAXNAMELEN];
};

#define IP_SET_OP_UNBIND_SET	0x00000105	/* Unbind an IP from a set */
/* Uses ip_set_req_bind, with type speficic addage
 * index = 0 means unbinding for all sets */

#define IP_SET_OP_TEST_BIND_SET	0x00000106	/* Test binding an IP to a set */
/* Uses ip_set_req_bind, with type specific addage */

/* Multiple shots operations: list, save, restore.
 *
 * - check kernel version and query the max number of sets
 * - get the basic information on all sets
 *   and size required for the next step
 * - get actual set data: header, data, bindings
 */

/* Get max_sets and the index of a queried set
 */
#define IP_SET_OP_MAX_SETS	0x00000020
struct ip_set_req_max_sets {
	unsigned op;
	unsigned version;
	ip_set_id_t max_sets;		/* max_sets */
	ip_set_id_t sets;		/* real number of sets */
	union ip_set_name_index set;	/* index of set if name used */
};

/* Get the id and name of the sets plus size for next step */
#define IP_SET_OP_LIST_SIZE	0x00000201
#define IP_SET_OP_SAVE_SIZE	0x00000202
struct ip_set_req_setnames {
	unsigned op;
	ip_set_id_t index;		/* set to list/save */
	size_t size;			/* size to get setdata/bindings */
	/* followed by sets number of struct ip_set_name_list */
};

struct ip_set_name_list {
	char name[IP_SET_MAXNAMELEN];
	char typename[IP_SET_MAXNAMELEN];
	ip_set_id_t index;
	ip_set_id_t id;
};

/* The actual list operation */
#define IP_SET_OP_LIST		0x00000203
struct ip_set_req_list {
	IP_SET_REQ_BYINDEX;
	/* sets number of struct ip_set_list in reply */
};

struct ip_set_list {
	ip_set_id_t index;
	ip_set_id_t binding;
	u_int32_t ref;
	size_t header_size;	/* Set header data of header_size */
	size_t members_size;	/* Set members data of members_size */
	size_t bindings_size;	/* Set bindings data of bindings_size */
};

struct ip_set_hash_list {
	ip_set_ip_t ip;
	ip_set_id_t binding;
};

/* The save operation */
#define IP_SET_OP_SAVE		0x00000204
/* Uses ip_set_req_list, in the reply replaced by
 * sets number of struct ip_set_save plus a marker
 * ip_set_save followed by ip_set_hash_save structures.
 */
struct ip_set_save {
	ip_set_id_t index;
	ip_set_id_t binding;
	size_t header_size;	/* Set header data of header_size */
	size_t members_size;	/* Set members data of members_size */
};

/* At restoring, ip == 0 means default binding for the given set: */
struct ip_set_hash_save {
	ip_set_ip_t ip;
	ip_set_id_t id;
	ip_set_id_t binding;
};

/* The restore operation */
#define IP_SET_OP_RESTORE	0x00000205
/* Uses ip_set_req_setnames followed by ip_set_restore structures
 * plus a marker ip_set_restore, followed by ip_set_hash_save
 * structures.
 */
struct ip_set_restore {
	char name[IP_SET_MAXNAMELEN];
	char typename[IP_SET_MAXNAMELEN];
	ip_set_id_t index;
	size_t header_size;	/* Create data of header_size */
	size_t members_size;	/* Set members data of members_size */
};

static inline int bitmap_bytes(ip_set_ip_t a, ip_set_ip_t b)
{
	return 4 * ((((b - a + 8) / 8) + 3) / 4);
}


#endif /*_IP_SET_H*/
