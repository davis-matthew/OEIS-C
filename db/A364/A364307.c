/*
 * A364307  Numbers k such that k, k+1 and k+2 have exactly 2 distinct prime
 *          factors.
 * https://oeis.org/A364307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1715, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9651 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A364307:
   numbers k such that k, k+1, and k+2 each have exactly 2 distinct prime factors. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/omega.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    /* last integer k that we have inspected (k-1 has been checked) */
    static uint64_t last_seen = 0;

    if (n == 0)                 /* impossible for OEIS offset but safe */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        uint64_t k = last_seen + 1;            /* next candidate */

        if (omega(k)      == 2 &&
            omega(k + 1)  == 2 &&
            omega(k + 2)  == 2)
        {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
        }

        last_seen = k;
    }

    return cache[n - 1];
}
