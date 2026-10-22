/*
 * A399582  Numbers k not divisible by 3 such that at least one of 3+2k and
 *          3+4k is composite.
 * https://oeis.org/A399582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k>0 with k%3!=0 and (3+2k or 3+4k is
 *            composite)
 * Range:     u8 n <= 23, u16 n <= 7203, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterative search with memoization; uses is_prime_big for primality checks. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored in cache */
    static idx_t next_k = 1;        /* next k to test for inclusion */

    if (n == 0)
        return 0;   /* undefined, but safe to return zero */

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* search for next qualifying k */
        while (true) {
            idx_t k = next_k++;
            if (k % 3 == 0)
                continue;    /* skip multiples of 3 */

            seq_t val1 = 3 + (seq_t)2 * k;
            seq_t val2 = 3 + (seq_t)4 * k;

            if (!is_prime_big(val1) || !is_prime_big(val2)) {
                /* Store the qualifying k */
                if (cache_size == 0) {
                    cache = (seq_t *)malloc(sizeof(seq_t));
                } else {
                    cache = (seq_t *)realloc(cache,
                        (cache_size + 1) * sizeof(seq_t));
                }
                cache[cache_size++] = (seq_t)k;
                break;   /* move to next term needed */
            }
        }
    }

    return cache[n - 1];
}
