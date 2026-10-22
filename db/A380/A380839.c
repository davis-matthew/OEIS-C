/*
 * A380839  Numerators of J(n) = Product_{p|n, p odd prime} (p - 1)/(p - 2).
 * https://oeis.org/A380839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∏_{p∣n, p odd prime} (p-1)/(p-2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Empty product for n = 1 returns 1 */
    if (n <= (idx_t)1)
        return (seq_t)1;

    seq_t num = (seq_t)1;   /* running numerator */
    seq_t den = (seq_t)1;   /* running denominator */

    /* Factor n using the provided helper */
    uint64_t nn   = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];      /* exponent array is not used locally */
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2)                /* skip even prime 2 */
            continue;

        /* individual factor (p-1)/(p-2) */
        seq_t a_f = (seq_t)(p - 1);
        seq_t b_f = (seq_t)(p - 2);

        /* reduce this factor fraction */
        seq_t g = gcd(a_f, b_f);
        a_f /= g;
        b_f /= g;

        /* cancel a_f with current denominator */
        g = gcd(a_f, den);
        a_f /= g;
        den   /= g;

        /* cancel current numerator with b_f */
        g = gcd(num, b_f);
        num   /= g;
        b_f   /= g;

        /* multiply the reduced contributions */
        num *= a_f;
        den *= b_f;

        /* final cancellation, guarantees den remains 1 at the end */
        g = gcd(num, den);
        num /= g;
        den /= g;
    }

    return num;
}
