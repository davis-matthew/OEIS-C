/*
 * A297274  Numbers whose base-11 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 5967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9964 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297274: numbers whose base-11 digits have equal down-variation and up-variation.
   Computed by brute force search and memoized. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: true if the base‑11 down‑variation equals the up‑variation of n */
static bool digit_property(seq_t n)
{
    if (n == 0)
        return true;                /* trivially true for 0 */

    seq_t temp = n;
    seq_t prev = temp % 11;
    temp /= 11;

    seq_t dv = 0;
    seq_t uv = 0;
    while (temp > 0) {
        seq_t cur = temp % 11;
        if (cur > prev) {
            dv += (cur - prev);
        } else if (cur < prev) {
            uv += (prev - cur);
        }
        prev = cur;
        temp /= 11;
    }
    return dv == uv;
}

seq_t a(idx_t n)
{
    /* static memoization data */
    static seq_t *cache = NULL;        /* array of found terms */
    static size_t cache_cap = 0;        /* capacity of the array */
    static idx_t cache_size = 0;        /* number of terms stored */
    static seq_t next_candidate = 1;   /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 256;                /* initial capacity */
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();            /* unrecoverable if out of memory */
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t c = next_candidate;
        while (!digit_property(c))
            ++c;                       /* find next valid term */

        /* grow cache if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[(size_t)cache_size] = c;
        ++cache_size;
        next_candidate = c + 1;
    }

    return cache[(size_t)(n - 1)];      /* sequence is 1‑based */
}
