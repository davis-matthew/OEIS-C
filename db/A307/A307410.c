/*
 * A307410  Numerators of the product in the singular series.
 * https://oeis.org/A307410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = numerator of Product_{p|n;p>2} (p-2)/(p-1) (OEIS A307410)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    /* factor n into distinct prime factors */
    static const size_t MAX_FA = 64;          /* enough for 64-bit integers */
    uint64_t primes[MAX_FA];
    uint64_t exps[MAX_FA];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t num = 1;
    seq_t den = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;               /* skip prime 2 */
        seq_t pm2 = (seq_t)p - 2;            /* p-2  */
        seq_t pm1 = (seq_t)p - 1;            /* p-1  */
        num *= pm2;
        den *= pm1;
    }

    /* reduce the fraction */
    seq_t g = gcd(num, den);
    num /= g;
    return num;
}
