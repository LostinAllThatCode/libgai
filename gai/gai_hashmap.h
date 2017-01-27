/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_HASHMAP_H_

#include "gai_types.h"
#include "gai_utils.h"

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

/*
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */
typedef void *any_t;

/*
 * PFany is a pointer to a function that can take two any_t arguments
 * and return an integer. Returns status code..
 */
typedef int (*PFany)(any_t, any_t);

/*
 * map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

#define INITIAL_SIZE      256
#define MAX_CHAIN_LENGTH  8

typedef struct
{
	char* key;
	int in_use;
	any_t data;
} gaiHashMapElement;

typedef struct {
	int table_size;
	int size;
	gaiHashMapElement *data;
} gaiHashMap;

GAI_DEF map_t gaiHashMapCreate  ();
GAI_DEF int   gaiHashMapIterate (map_t in, PFany f, any_t item);
GAI_DEF int   gaiHashMapPut     (map_t in, char* key, any_t value);
GAI_DEF int   gaiHashMapGet     (map_t in, char* key, any_t *arg);
GAI_DEF int   gaiHashMapRemove  (map_t in, char* key);
GAI_DEF void  gaiHashMapFree    (map_t in);
GAI_DEF int   gaiHashMapLength  (map_t in);

#ifdef GAI_HASHMAP_IMPLEMENTATION
#   include "gai_hashmap.cc"
#endif

#define _GAI_HASHMAP_H_
#endif
