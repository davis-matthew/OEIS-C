/*
 * A399502  Consider a 3D sandpile model where each site with 3 or more
 *          grains, say at location (x, y, z), topples and transfers one grain
 *          of sand to the sites at locations (x+1, y, z), (x, y+1, z) and (x,
 *          y, z+1); a(n) gives the number of nonempty sites after
 *          stabilization of a configuration starting with n...
 * https://oeis.org/A399502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   push_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399502 – 3‑D directed sandpile
 *
 * Computation:  For n grains at the origin, repeatedly
 * toppling any site with at least 3 grains (sending one grain
 * to the positive x, y, and z neighbours).  After all sites
 * have at most 2 grains, the number of non‑empty sites is a(n).
 * This is the definition of OEIS A399502.
 *
 * The implementation simulates the process using a hash table
 * (open addressing) for the lattice sites and a dynamic stack
 * for unstable sites.  It works for any seq_t / idx_t supplied
 * by the build and keeps all intermediate arithmetic within
 * seq_t as required.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/push_key.h"

struct Entry { uint64_t key; seq_t val; };

/* Marker for an empty table entry */
static const uint64_t EMPTY_KEY = UINT64_MAX;

/* Return pointer to value for given key, inserting if absent */
static inline seq_t *get_val(uint64_t key, struct Entry *table, size_t mask)
{
    size_t i = (key * 11400714819323198485ULL) & mask;
    for (;; i = (i + 1) & mask) {
        if (table[i].key == key)
            return &table[i].val;
        if (table[i].key == EMPTY_KEY) {
            table[i].key = key;
            table[i].val = 0;
            return &table[i].val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* a(0) = 0 */

    /* Allocate hash table, power‑of‑two sized, 4× n entries to keep load factor low */
    size_t cap = 16;
    while (cap < (size_t)n * 4 + 64) cap <<= 1;
    struct Entry *table = (struct Entry *)calloc(cap, sizeof(struct Entry));
    if (!table) return 0;  /* allocation failure – unlikely in test harness */
    for (size_t i = 0; i < cap; ++i) table[i].key = EMPTY_KEY;
    size_t mask = cap - 1;

    /* Dynamic stack for unstable sites */
    size_t qcap = 16;
    uint64_t *qbuf = (uint64_t *)calloc(qcap, sizeof(uint64_t));
    size_t qsize = 0;
    if (!qbuf) { free(table); return 0; }

    /* Origin coordinates encoded into key: 21 bits each */
    uint64_t origin = 0ULL;
    seq_t *pv = get_val(origin, table, mask);
    *pv = (seq_t)n;
    if (n >= 3) push_key(origin, &qbuf, &qsize, &qcap);

    /* Main toppling loop */
    while (qsize > 0) {
        uint64_t key = qbuf[--qsize];
        seq_t *pval = get_val(key, table, mask);
        seq_t val = *pval;
        if (val < 3) continue;          /* may have become stable earlier */

        *pval = val - 3;                /* perform one toppling */

        /* Decode coordinates: 21 bits each */
        uint32_t x = (uint32_t)(key >> 42);
        uint32_t y = (uint32_t)((key >> 21) & ((1ULL << 21) - 1));
        uint32_t z = (uint32_t)(key & ((1ULL << 21) - 1));

        /* Neighbor (x+1, y,   z) */
        uint64_t nkey1 = (((uint64_t)(x + 1)) << 42)
                     | ((uint64_t)y << 21)
                     | ((uint64_t)z);
        seq_t *np1 = get_val(nkey1, table, mask);
        *np1 += 1;
        if (*np1 >= 3) push_key(nkey1, &qbuf, &qsize, &qcap);

        /* Neighbor (x,   y+1, z) */
        uint64_t nkey2 = (((uint64_t)x) << 42)
                     | (((uint64_t)(y + 1)) << 21)
                     | ((uint64_t)z);
        seq_t *np2 = get_val(nkey2, table, mask);
        *np2 += 1;
        if (*np2 >= 3) push_key(nkey2, &qbuf, &qsize, &qcap);

        /* Neighbor (x,   y,   z+1) */
        uint64_t nkey3 = (((uint64_t)x) << 42)
                     | (((uint64_t)y) << 21)
                     | ((uint64_t)(z + 1));
        seq_t *np3 = get_val(nkey3, table, mask);
        *np3 += 1;
        if (*np3 >= 3) push_key(nkey3, &qbuf, &qsize, &qcap);

        /* If the current site is still unstable, schedule another toppling */
        if (*pval >= 3) push_key(key, &qbuf, &qsize, &qcap);
    }

    /* Count non‑empty sites */
    size_t count = 0;
    for (size_t i = 0; i < cap; ++i) {
        if (table[i].key != EMPTY_KEY && table[i].val != 0)
            ++count;
    }

    free(table);
    free(qbuf);
    return (seq_t)count;
}
