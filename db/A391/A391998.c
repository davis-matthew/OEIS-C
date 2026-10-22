/*
 * A391998  Composite numbers k such that k + 1 is not a power of an odd prime
 *          and 2*k + 1 is not a prime power.
 * https://oeis.org/A391998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite integer k such that (1) k + 1 is not a
 *            power of an odd prime, (2) 2*k + 1 is not a prime power
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Test if n is a prime power.  If base_out is non‑NULL, store the prime base. */
static inline bool is_prime_power_uint64(uint64_t n, uint64_t *base_out)
{
    if (n <= 1)
        return false;

    /* Find the smallest prime divisor of n. */
    uint64_t d = 2;
    while (d * d <= n && n % d != 0)
        ++d;
    if (n % d != 0) {        /* n is prime */
        if (base_out)
            *base_out = n;
        return true;
    }

    /* Remove all factors of d from n. */
    uint64_t m = n;
    while (m % d == 0)
        m /= d;
    if (m == 1) {            /* n is a pure power of d */
        if (base_out)
            *base_out = d;
        return true;
    }
    return false;           /* n has at least two distinct prime factors */
}

/* Sequence A391998. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; n=0 is unused */

    idx_t count = 0;
    uint64_t k = 4;        /* first composite number */

    while (1) {
        if (!is_prime_small(k)) {                 /* k is composite */
            bool bad = false;
            uint64_t kp1 = k + 1;
            uint64_t kp2 = 2 * k + 1;
            uint64_t base;

            /* k + 1 must NOT be a power of an odd prime. */
            if (is_prime_power_uint64(kp1, &base) && (base & 1ULL))
                bad = true;

            /* 2*k + 1 must NOT be a prime power. */
            if (!bad && is_prime_power_uint64(kp2, &base))
                bad = true;

            if (!bad) {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }
}
