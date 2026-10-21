/*
 * A282795  Start with n. If n is 1 or a prime, stop. Otherwise, add the prime
 *          factors of n (with repetition) to n, and repeat until reaching a
 *          prime, when we stop. If no prime is ever reached, a(n) = -1.
 * https://oeis.org/A282795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by starting with n and repeatedly adding the
 *            sum of its prime factors (with multiplicity) until the value
 *            becomes 1 or a prime. The resulting 1 or prime is returned
 * Range:     u8 n <= 65, u16 n <= 6032, u32 n <= 9147, u64 n <= 9147,
 *            u128 n <= 9147, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"       /* is_prime_big(seq_t) */
 
/* compute the sum of prime factors of a 64‑bit unsigned integer
   (with multiplicity) */
static uint64_t sum_prime_factors_uint64(uint64_t x)
{
    uint64_t sum = 0;
    /* handle factor 2 separately */
    while ((x & 1ULL) == 0) {
        sum += 2ULL;
        x >>= 1;
    }
    /* trial divide odd factors */
    uint64_t p = 3ULL;
    while (p * p <= x) {
        while (x % p == 0) {
            sum += p;
            x /= p;
        }
        p += 2;
    }
    if (x > 1ULL)
        sum += x;
    return sum;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)          /* harmless guard for out‑of‑range indices */
        return 0;

    seq_t cur = (seq_t)n;

    /* iterate until we reach 1 or a prime */
    while (true) {
        if (cur == (seq_t)1 || is_prime_big(cur))
            return cur;

        /* add sum of prime factors (handled as uint64_t) */
        uint64_t add = sum_prime_factors_uint64((uint64_t)cur);
        cur += (seq_t)add;
    }
}
