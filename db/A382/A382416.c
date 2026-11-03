/*
 * A382416  Numbers with at least one zero in their base-6 representation.
 * https://oeis.org/A382416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth nonnegative integer whose base‑6 representation
 *            contains at least one zero digit
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9782 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a base‑6 representation of a number contains a zero digit. */
static bool has_zero_base6(seq_t x)
{
    if (x == (seq_t)0) return true;               /* 0 itself has a zero */
    while (x > (seq_t)0) {
        if ((x % (seq_t)6) == (seq_t)0) return true;
        x /= (seq_t)6;
    }
    return false;
}

/* Return the nth term (n ≥ 1).  Cache results so repeated calls are fast. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored in cache */
    static size_t cap = 0;     /* allocated capacity of cache array */

    if (n == 0) return (seq_t)0;   /* just in case; offset is 1 */

    /* Ensure we have at least n terms ready */
    if (cached < n) {
        /* Initial allocation */
        if (cached == 0) {
            cap = 10;
            cache = mem_new(seq_t, cap);
            if (!cache) return (seq_t)0;   /* allocation failure */
            cache[0] = (seq_t)0;          /* the first term */
            cached = 1;
            /* start searching from 1 */
            seq_t next = (seq_t)1;
            while (cached < n) {
                /* Grow cache if needed */
                if (cap <= cached) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return (seq_t)0;
                    cache = tmp;
                    cap = new_cap;
                }
                if (has_zero_base6(next)) {
                    cache[cached++] = next;
                }
                next += (seq_t)1;
            }
        } else {
            /* Continue from the last stored value */
            seq_t next = cache[cached - 1] + (seq_t)1;
            while (cached < n) {
                /* Grow cache if needed */
                if (cap <= cached) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return (seq_t)0;
                    cache = tmp;
                    cap = new_cap;
                }
                if (has_zero_base6(next)) {
                    cache[cached++] = next;
                }
                next += (seq_t)1;
            }
        }
    }
    return cache[n - 1];
}
