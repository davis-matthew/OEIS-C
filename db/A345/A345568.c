/*
 * A345568  Numbers that are the sum of seven fourth powers in two or more
 *          ways.
 * https://oeis.org/A345568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ecdfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345568: Numbers that are the sum of seven fourth powers in two or more ways.
   There is no simple closed‑form; the values are obtained by enumerating all unordered
   7‑tuples (a1≤a2≤…≤a7) of positive integers and counting the sums that occur at
   least twice. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ecdfs.h"

/* ---------- static data --------------------------------------------------- */

static const idx_t REQUIRED_MAX = 10000;             /* maximum result index needed */
static seq_t *seq_cache = NULL;                     /* array of found terms       */
static size_t seq_cache_len = 0;                    /* number of stored terms     */

/* ---------- helper – recursive enumeration of 7‑tuples --------------------- */

/* ---------- build the cache --------------------------------------------- */
static void build_cache(void)
{
    int base = 35;                     /* initial upper bound on the fourth‑power roots */
    const int MAX_TRIES = 10;          /* safety limit for increasing base */
    int tries = 0;

    while (tries < MAX_TRIES) {
        unsigned int max_sum = 7u * (unsigned int)(base*base*base*base);
        unsigned int *counts = (unsigned int *)calloc((size_t)max_sum + 1, sizeof(unsigned int));
        if (!counts) exit(1);          /* allocation failure */

        unsigned int pow4[base + 1];
        pow4[0] = 0;
        for (int i = 1; i <= base; ++i)
            pow4[i] = (unsigned int)(i * i * i * i);

        ecdfs(0, 1, 0, base, pow4, counts, max_sum);

        /* determine the number of sums with at least two representations */
        size_t total = 0;
        for (unsigned int s = 1; s <= max_sum; ++s)
            if (counts[s] >= 2) ++total;

        if (total >= REQUIRED_MAX) {          /* we have enough terms */
            seq_cache_len = total;
            seq_cache = (seq_t *)malloc(seq_cache_len * sizeof(seq_t));
            if (!seq_cache) exit(1);

            size_t idx = 0;
            for (unsigned int s = 1; s <= max_sum && idx < seq_cache_len; ++s)
                if (counts[s] >= 2)
                    seq_cache[idx++] = (seq_t)s;
            free(counts);
            return;
        }

        free(counts);
        /* not enough terms – increase the search range and try again */
        base += 5;
        ++tries;
    }

    /* If we reach here we could not find enough terms – abort */
    exit(1);
}

/* ---------- public API --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0, guard against underflow */
    if (!seq_cache) build_cache();           /* lazily compute on first call */
    if (n - 1 >= seq_cache_len) exit(1);     /* sequence not long enough – should not happen */
    return seq_cache[n - 1];
}
