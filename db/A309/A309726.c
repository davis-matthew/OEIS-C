/*
 * A309726  Numbers k such that k^2 - 12 is prime.
 * https://oeis.org/A309726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309726: a(n) = the nth integer k such that k^2 - 12 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t sz = 0, cap = 0;
    static seq_t candidate = 5;

    while (sz < n) {
        /* Test candidate: k^2 - 12 must be prime */
        seq_t val = candidate * candidate;
        val -= (seq_t)12;

        if (is_prime_big(val)) {
            /* Grow cache if necessary */
            if (sz == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newp) abort();
                cache = newp;
                cap = newcap;
            }
            cache[sz++] = candidate;
        }

        /* Advance to the next candidate that is odd and not divisible by 3 */
        candidate += (seq_t)2;
        while ((candidate % (seq_t)3) == (seq_t)0)
            candidate += (seq_t)2;
    }

    return cache[n - 1];
}
