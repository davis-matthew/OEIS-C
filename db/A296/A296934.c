/*
 * A296934  Rational primes that decompose in the field Q(sqrt(7)).
 * https://oeis.org/A296934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3269, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≠ 2,7 that split in Q(sqrt(7))
   A(p) = 1 iff (7/p) = 1  ⇔  p ≡ 1,3,9,19,25,27 (mod 28).  */

/*  The function caches the sequence values so that any ordering of
    calls to a(idx_t) works efficiently.  It uses the supplied
    is_prime(idx_t) routine from the utils library.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* stored sequence values       */
    static size_t cache_cap = 0;    /* cache capacity               */
    static idx_t  cache_cnt  = 0;   /* number of values cached      */
    static idx_t  cand       = 3;   /* next odd candidate to test   */

    /* generate sequence values until we have at least n entries   */
    while (cache_cnt < n) {
        if (is_prime(cand)) {
            idx_t r = cand % 28;
            if (r == 1 || r == 3 || r == 9 || r == 19 || r == 25 || r == 27) {
                /* grow cache if necessary */
                if (cache_cnt >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_cnt++] = (seq_t)cand;
            }
        }
        cand += 2;   /* skip even numbers */
    }
    return cache[n - 1];
}
