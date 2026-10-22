/*
 * A307792  Triangular numbers t(n) such that n / (product of decimal digits
 *          of t(n)) is an integer.
 * https://oeis.org/A307792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_prod_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A307792: a(n) is the nth triangular number
   T_k = k(k+1)/2 such that k is divisible by the product of the
   decimal digits of T_k.  The implementation searches k = 1,2,…
   incrementally, stores the found terms in a static cache, and
   returns the requested element.  The algorithm runs in O(n) time
   per new element and is fully generic for the integer types
   seq_t and idx_t supplied by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_prod_limit.h"

/* Helper: compute product of decimal digits of x, with an early
   exit if the product would exceed the value of k.  If the product
   becomes larger than k, the function returns a value greater than
   k (specifically k+1).  This allows the caller to avoid any
   conversion to seq_t when the product cannot divide k. */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;        /* array of found terms */
    static size_t  ccap   = 0;           /* current capacity of cache   */
    static idx_t   found  = 0;           /* number of terms already found */
    static idx_t   last_k = 0;           /* last k tested               */

    /* fast return if the result is already cached */
    if (n <= found) return cache[n - 1];

    /* grow cache if needed */
    if (found == ccap) {
        size_t new_cap = ccap ? ccap * 2 : 16;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (cache) {
            for (size_t i = 0; i < ccap; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        ccap = new_cap;
    }

    /* generate terms until we have n of them */
    while (found < n) {
        idx_t k = ++last_k;                      /* next k to test */

        seq_t k_seq = (seq_t)k;
        seq_t T = (k_seq * (k_seq + (seq_t)1)) / (seq_t)2;

        /* compute product of decimal digits of T, stop if too large */
        uint64_t prod = digit_prod_limit(T, k);

        if (prod != 0 && prod <= (uint64_t)k) {
            /* k is divisible by prod */
            if (k % prod == 0) {
                cache[found++] = T;
                continue;          /* move to next k */
            }
        }
        /* otherwise not a valid term – keep searching */
    }

    return cache[n - 1];
}
