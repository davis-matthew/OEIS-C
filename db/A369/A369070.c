/*
 * A369070  a(n) = 1 if there is at least one prime power p^e in the prime
 *          factorization of n such that p|e, otherwise 0.
 * https://oeis.org/A369070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists a prime power p^e in the prime
 *            factorization of n such that p divides e; 0 otherwise
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 140 terms through n = 9728 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return 1 if any exponent e of a prime p in n satisfies p | e, else 0. */
static bool has_condition(uint64_t n)
{
    if (n <= 1)           /* 1 has no prime factors */
        return false;

    uint64_t m = n;

    /* Handle factor 2 separately, checking if exponent is even. */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {   /* while m divisible by 2 */
        m >>= 1;
        ++e;
    }
    if (e && (e % 2ULL == 0))
        return true;

    /* Test odd prime factors up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {   /* count exponent of p */
                m /= p;
                ++e;
            }
            if (e % p == 0)
                return true;
        }
    }
    /* If remaining m > 1, it is a prime factor with exponent 1. */
    /* Exponent 1 is not divisible by any prime > 1, so no need to check. */
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    return has_condition((uint64_t)n) ? (seq_t)1 : (seq_t)0;
}
