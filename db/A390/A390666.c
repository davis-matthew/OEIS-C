/*
 * A390666  The sum of the unitary divisors of the smallest number whose cube
 *          is divisible by n.
 * https://oeis.org/A390666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e∥n} (1 + p^ceil(e/3))
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: “a(n) is the sum of the unitary divisors of the minimal
   integer m with m^3 divisible by n” */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t temp = n;
    seq_t result = 1;

    /* factor 2 separately */
    idx_t e = 0;
    while ((temp & 1ULL) == 0) {
        e++;
        temp >>= 1;
    }
    if (e) {
        idx_t exp3 = (e + 2) / 3;          /* ceil(e/3) */
        seq_t powp = 1;
        for (idx_t i = 0; i < exp3; ++i)
            powp *= (seq_t)2;
        result *= (1 + powp);
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            idx_t exp3 = (e + 2) / 3;
            seq_t powp = 1;
            for (idx_t j = 0; j < exp3; ++j)
                powp *= (seq_t)p;
            result *= (1 + powp);
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        seq_t powp = (seq_t)temp;  /* exponent e = 1 -> powp = p */
        result *= (1 + powp);
    }

    return result;
}
