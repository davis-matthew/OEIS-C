/*
 * A295265  Numbers m such that sum of its i first divisors equals the sum of
 *          its j first non-divisors for some i, j.
 * https://oeis.org/A295265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_in_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A295265.  For a given n, find the nth integer m such that
 * there exist indices i,j with
 *   sum_{k=1..i} d_k(m) = sum_{k=1..j} nd_k(m),
 * where d_k(m) are the divisors of m in increasing order and nd_k(m)
 * the non‑divisors of m in increasing order.
 *
 * The implementation stores the numbers found in a cache and expands it
 * until it contains at least n terms.  Each candidate m is tested by
 * computing its divisors, the prefix sums of those divisors, and
 * incrementally summing the non‑divisors until the sums match.  All
 * arithmetic is performed on seq_t; idx_t is assumed to be an unsigned
 * 64‑bit integer type.  The code is valid C11 and C++17.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_in_sequence.h"

/* ------------------------------------------------------------------ */
/*  Helper that determines whether m belongs to the sequence.         */

/* ------------------------------------------------------------------ */
/*  Cached enumeration of the sequence.                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of found numbers            */
    static size_t cache_cap = 0;   /* allocated capacity                */
    static size_t cache_len = 0;   /* number of entries in the cache    */
    static idx_t max_checked = 0;  /* largest m already examined        */

    if (n == 0) return (seq_t)0;    /* not used but harmless             */

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    while (cache_len < (size_t)n) {
        ++max_checked;
        if (is_in_sequence(max_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
    return cache[n - 1];
}
