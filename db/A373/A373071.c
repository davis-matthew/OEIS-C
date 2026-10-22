/*
 * A373071  Integers k such that k-1 is a multiple of Sum_{prime|k}(prime-1).
 * https://oeis.org/A373071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 7090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_prime_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A373071: Integers k such that k-1 is a multiple of
 *   Sum_{prime | k}(prime-1).
 *
 * The algorithm builds the sequence by testing consecutive integers
 *   starting at 2.  For each candidate k it factors k in O(sqrt(k))
 *   time and computes the sum of (p-1) over its distinct prime
 *   divisors.  If (k-1) mod that sum is zero, k is accepted.
 *
 * The implementation uses a small cache to store the sequence values
 *   found so far, allowing arbitrary ordering of calls to `a`.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sum_prime_minus_one.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* Undefined for n=0; return 0 to keep C code safe. */
    }

    /* Static cache for sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next integer to test */
    static idx_t next_k = 2;

    while (cache_len < (size_t)n) {
        uint64_t sum = sum_prime_minus_one((uint64_t)next_k);
        if ((next_k - 1U) % sum == 0) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache,
                                               cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
