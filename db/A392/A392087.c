/*
 * A392087  The unitary totient of the smallest square divisible by n.
 * https://oeis.org/A392087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e∥n} (p^(e + (e mod 2)) - 1) (multiplicative)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* prime factors and exponents of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t e2  = e + (e & 1);          /* e + (e mod 2) */
        seq_t  term  = 1;

        /* compute p^e2 */
        for (uint64_t j = 0; j < e2; ++j)
            term *= (seq_t)p;

        term -= 1;            /* p^e2 - 1 */
        result *= term;       /* multiply into product */
    }
    return result;
}
