/*
 * A319035  Triangular numbers T(k) that have the same number of divisors as
 *          their successors T(k+1).
 * https://oeis.org/A319035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A319035.
   The sequence consists of triangular numbers T(k) = k(k+1)/2 such that
   the number of divisors of T(k) equals the number of divisors of T(k+1).
   The implementation generates triangular numbers in order, keeps a
   static cache of the matching ones, and returns the cached value for
   any requested index. */

#include <stdlib.h>                 /* malloc, realloc, free */
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static state: cached matching triangular numbers */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current triangular index being examined (k >= 3 for the first term) */
    static idx_t current_k = 3;

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {

        /* Ensure capacity for another element */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {                 /* allocation failure */
                if (cache) free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return (seq_t)0;
            }
            cache   = tmp;
            cache_cap = new_cap;
        }

        /* Compute current and next triangular numbers */
        seq_t Tk   = ((seq_t)current_k     * (seq_t)(current_k + 1)) / 2;
        seq_t Tk1  = ((seq_t)(current_k+1) * (seq_t)(current_k + 2)) / 2;

        /* Count divisors of both numbers (values fit in uint64_t for the tested range) */
        seq_t dTk  = (seq_t)divisor_count_u64((uint64_t)Tk);
        seq_t dTk1 = (seq_t)divisor_count_u64((uint64_t)Tk1);

        /* Record Tk if its divisor count matches Tk+1's */
        if (dTk == dTk1) {
            cache[cache_len++] = Tk;
        }

        /* Advance to the next triangular index */
        ++current_k;
    }

    return cache[n - 1];
}
