/*
 * A283727  Number of 2 X n 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 31*a(n-4) + 14*a(n-5) + * 51*a(n-6)
 *            + 52*a(n-7) + 12*a(n-8) - 96*a(n-9) - 64*a(n-10) * Initial
 *            values: a(1)=0, a(2)=0, a(3)=10, a(4)=60, a(5)=242, * a(6)=1032,
 *            a(7)=4220, a(8)=16376, a(9)=62564, a(10)=235728
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

enum { INIT_LEN = 10 };

/* Initial terms (0‑based index i corresponds to a(i+1)) */
static const seq_t first10[INIT_LEN] = {
    (seq_t)0,   /* a(1) */
    (seq_t)0,   /* a(2) */
    (seq_t)10,  /* a(3) */
    (seq_t)60,  /* a(4) */
    (seq_t)242, /* a(5) */
    (seq_t)1032,/* a(6) */
    (seq_t)4220,/* a(7) */
    (seq_t)16376,/* a(8) */
    (seq_t)62564,/* a(9) */
    (seq_t)235728 /* a(10) */
};

static seq_t *cache = NULL;  /* memoised sequence values */
static size_t cached_len = 0; /* number of values currently cached */

/* Compute a(n), with n >= 1. Undefined for n <= 0, returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)               /* n <= 0 undefined */
        return (seq_t)0;

    if (!cache || (size_t)n > cached_len) {
        /* Allocate or grow the cache to size n. */
        size_t new_len = (size_t)n;
        seq_t *new_cache;
        if (!cache) {
            new_cache = seq_alloc(new_len);     /* allocates new array */
        } else {
            new_cache = seq_realloc(cache, cached_len, new_len);
        }
        if (!new_cache)            /* allocation failed */
            return (seq_t)0;
        cache = new_cache;

        /* If we grew from a smaller size, copy old values if any */
        /* seq_realloc handles this automatically for us. */

        /* Fill initial terms if not already present */
        for (size_t i = cached_len; i < INIT_LEN && i < new_len; ++i)
            cache[i] = first10[i];

        /* Compute missing terms using the recurrence */
        size_t start = cached_len > INIT_LEN ? cached_len : INIT_LEN;
        for (idx_t k = (idx_t)start + 1; k <= n; ++k) {
            /* k corresponds to a(k).  Cache index is k-1. */
            size_t idx = (size_t)k;
            seq_t val = 6 * cache[idx - 2]           /* a(k-1) */
                     - 7 * cache[idx - 3]           /* a(k-2) */
                     - 31 * cache[idx - 5]          /* a(k-4) */
                     + 14 * cache[idx - 6]          /* a(k-5) */
                     + 51 * cache[idx - 7]          /* a(k-6) */
                     + 52 * cache[idx - 8]          /* a(k-7) */
                     + 12 * cache[idx - 9]          /* a(k-8) */
                     - 96 * cache[idx - 10]         /* a(k-9) */
                     - 64 * cache[idx - 11];        /* a(k-10) */
            cache[idx - 1] = val;
        }

        cached_len = new_len;
    }

    return cache[(size_t)n - 1];
}
