/*
 * A267543  Primes p such that 3*p+1 is divisible by the sum of digits of
 *          2*p+1.
 * https://oeis.org/A267543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A267543 – primes p such that (3*p+1) is divisible by the sum of digits of (2*p+1).
   Implements a search-based computation. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t last_prime = 0; /* last prime examined */

    if (n == 0) return (seq_t)0; /* behavior for n < 1 is undefined */

    while (len < n) {
        /* find the next prime greater than last_prime */
        last_prime = next_prime(last_prime + 1);
        seq_t p = (seq_t)last_prime;

        seq_t sum = digit_sum((seq_t)( (seq_t)2 * p + 1 ));
        if (sum && ((seq_t)(3 * p + 1) % sum) == 0) {
            if (len == cap) {
                size_t new_cap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = p;
        }
    }
    return cache[n-1];
}
