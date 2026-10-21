/*
 * A282113  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j+1..k}{(i-j)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 8.
 * https://oeis.org/A282113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 4240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fulcrum_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A282113: Numbers with a “fulcrum” digit in base 8.
 *
 *  Property:
 *      Let n = (d_k … d_1)_8 with k ≥ 3 (d_k is the most significant
 *      and d_1 the least significant digit).
 *      The number satisfies the property if there exists an integer
 *      j with 2 ≤ j ≤ k−1 such that
 *
 *          Σ_{i=j+1..k} (i-j)·d_i  =  Σ_{i=1..j-1} (j-i)·d_i .
 *
 *  a(n) is the n‑th smallest positive integer that satisfies this
 *  property (the offset in OEIS is 1, so a(1) is the first term).
 *
 *  This implementation generates the sequence by simple brute force
 *  search, storing already found terms in a dynamically expandable
 *  static array.  Because the density of such numbers is fairly high,
 *  this method easily finds 10 000 terms well under the time limit.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/fulcrum_ok.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache      = NULL;   /* array of found terms   */
    static size_t  cache_cap = 0;      /* current capacity       */
    static size_t  cache_len = 0;      /* number of stored terms */
    static seq_t  last_checked = 0;    /* last candidate checked */

    /* If the requested term is already known, return it. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Ensure the cache has some storage. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate further terms until we have at least n. */
    seq_t candidate = last_checked;
    while (cache_len < (size_t)n) {
        ++candidate;                    /* next integer to test    */
        if (fulcrum_ok(candidate)) {    /* property satisfied?     */
            /* Grow cache if needed.  */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;                /* double */
                cache = (seq_t *)realloc(cache,
                    cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;    /* store term          */
        }
    }

    last_checked = candidate;      /* all numbers up to here have been inspected */
    return cache[n - 1];
}
