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
 * Validated: standard, pass, 78 terms through n = 9773 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static bool is_in_sequence(idx_t m)
{
    /* Collect all divisors of m.  The number of divisors of a 64‑bit
     * number never exceeds a few thousand even for highly composite
     * values, so a fixed array of size 2048 is safe.                     */
    enum { MAX_D = 2048 };
    idx_t divs[MAX_D];
    int dcnt = 0;

    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            if (dcnt < MAX_D) divs[dcnt++] = d;
            idx_t other = m / d;
            if (other != d && dcnt < MAX_D) divs[dcnt++] = other;
        }
    }

    /* Sort the divisor list ascending (small numbers have few divisors,
     * so an O(n²) sort is inexpensive).                           */
    for (int i = 0; i < dcnt - 1; ++i)
        for (int j = i + 1; j < dcnt; ++j)
            if (divs[i] > divs[j]) {
                idx_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    /* Prefix sums of the divisors.                                   */
    seq_t prefix[MAX_D];
    int pcnt = 0;
    seq_t sum_total = 0;
    for (int i = 0; i < dcnt; ++i) {
        sum_total += (seq_t)divs[i];
        prefix[pcnt++] = sum_total;
    }

    /* Incrementally sum non‑divisors and check against the divisor
     * prefix sums.  The moment the two sums become equal we have found
     * a valid pair (i,j).                                             */
    seq_t sum_non = 0;
    for (idx_t x = 1; x <= m; ++x) {
        if (m % x != 0) {
            sum_non += (seq_t)x;
            for (int i = 0; i < pcnt; ++i)
                if (sum_non == prefix[i])
                    return true;
        }
        if (sum_non > sum_total)
            break;
    }
    return false;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
    return cache[n - 1];
}
