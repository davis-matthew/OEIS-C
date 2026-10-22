/*
 * A372247  Lowest prime p in a ladder of 4 consecutive primes p, p+2, p+6,
 *          p+14.
 * https://oeis.org/A372247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that p, p+2, p+6 and p+14 are
 *            consecutive primes
 * Range:     u8 none, u16 n <= 20, u32 n <= 25, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t len    = 0;
    static uint64_t curr = 1;      /* start searching from the first prime */

    while (len < n) {
        /* advance to the next prime candidate */
        curr = next_prime(curr);

        uint64_t p   = curr;
        uint64_t p2  = p + 2;
        uint64_t p6  = p + 6;
        uint64_t p14 = p + 14;

        /* the required offsets must be prime */
        if (!is_prime_big((seq_t)p2) || !is_prime_big((seq_t)p6) ||
            !is_prime_big((seq_t)p14))
            continue;

        /* ensure the primes are consecutive */
        if (next_prime(p2 + 1) != p6) continue;
        if (next_prime(p6 + 1) != p14) continue;

        /* store the result, grow cache if necessary */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            ensure_capacity(new_cap, &cap, (void**)&cache, sizeof(seq_t));
        }
        cache[len++] = (seq_t)p;
    }

    return cache[n - 1];
}
