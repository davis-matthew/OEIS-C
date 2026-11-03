/*
 * A392599  a(n) = A276087(n) mod 360, where A276087 is the primorial base
 *          exp-function applied twice.
 * https://oeis.org/A392599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A276087(A276087(n)) mod 360
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 928, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 86 terms through n = 9538 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276087: primorial base exp-function */
#include <stddef.h>

/* exponentiation modulo 360 for small exponents */
static uint64_t pow_mod360(idx_t base, unsigned int exp)
{
    uint64_t r = 1;
    uint64_t b = base % 360;
    while (exp--) r = (r * b) % 360;
    return r;
}

/* first primes used for the primorial base */
static const idx_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
static const size_t NUM_PRIMES = sizeof(primes) / sizeof(primes[0]);

/* exponentiation for small exponents (fits in seq_t) */
static seq_t pow_small(seq_t base, unsigned int exp)
{
    seq_t r = 1;
    while (exp--) r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* first application: f(n) = A276087(n) */
    seq_t m = 1;          /* result of f(n) */
    seq_t weight = 1;     /* product of primes < current position */

    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        seq_t digit = 0;
        if (weight <= n) {
            digit = ((seq_t)n / weight) % (seq_t)primes[i];
        }
        if (digit) m *= pow_small((seq_t)primes[i], (unsigned int)digit);

        weight *= (seq_t)primes[i];
        if (weight > n) break;
    }

    /* second application: f(f(n)) mod 360 */
    uint64_t res = 1;

    weight = 1;
    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        seq_t digit = 0;
        if (weight <= m) {
            digit = (m / weight) % (seq_t)primes[i];
        }
        if (digit) {
            uint64_t pm = pow_mod360(primes[i], (unsigned int)digit);
            res = (res * pm) % 360;
        }

        weight *= (seq_t)primes[i];
        if (weight > m) break;
    }

    return (seq_t)res;
}
