/*
 * A369721  The sum of unitary divisors of the smallest cubefull number that
 *          is a multiple of n.
 * https://oeis.org/A369721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n} (1 + p^{max(3,e)})
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Multiply a base by itself 'exp' times into a seq_t.
   exp is assumed to be small (≤ the exponent in n). */
static seq_t pow_seq_uint(uint64_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    for (uint64_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    /* Special case: n = 1 has no prime factors → product = 1. */
    if (n == 1) return (seq_t)1;

    seq_t prod = (seq_t)1;
    uint64_t t = (uint64_t)n;

    /* Handle factor 2 separately to skip increments of 2 in the loop. */
    uint64_t e = 0;
    while (t % 2 == 0) {
        t /= 2;
        ++e;
    }
    if (e > 0) {
        uint64_t f = (e <= 2) ? 3 : e;
        seq_t p_pow = pow_seq_uint(2, f);
        prod *= (p_pow + (seq_t)1);
    }

    /* Iterate over odd candidates up to sqrt(t). */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            uint64_t f = (e <= 2) ? 3 : e;
            seq_t p_pow = pow_seq_uint(p, f);
            prod *= (p_pow + (seq_t)1);
        }
    }

    /* If what's left is a prime factor > sqrt(original n). */
    if (t > 1) {
        uint64_t f = 3; /* exponent of a single prime is 1 ≤ 2 → max(3,1)=3 */
        seq_t p_pow = pow_seq_uint(t, f);
        prod *= (p_pow + (seq_t)1);
    }

    return prod;
}
