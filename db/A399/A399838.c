/*
 * A399838  The sum of divisors d of n such that gcud(d, n/d) is odd, where
 *          gcud is the greatest common unitary divisor.
 * https://oeis.org/A399838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative. For n = ∏p^e * a(2^e) = 2^(e+1)-1, if e
 *            odd * a(2^e) = 2^(e+1)-1-2^(e/2), if e even * a(p^e) =
 *            (p^(e+1)-1)/(p-1) for odd prime p * a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* compute p^e for small e */
static seq_t pow_seq_uint(seq_t p, idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= p;
    return r;
}

/* geometric series sum (p^(e+1)-1)/(p-1), works for p=2 as well */
static seq_t sum_geom(seq_t p, idx_t e)
{
    seq_t pow = pow_seq_uint(p, e + 1);   /* p^(e+1) */
    seq_t num = pow - 1;                 /* p^(e+1)-1 */
    seq_t den = p - 1;                   /* p-1, equals 1 when p==2 */
    return num / den;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        idx_t e = (idx_t)ex[i];
        seq_t S;

        if (p == 2 && (e & 1) == 0) {          /* even exponent of 2 */
            seq_t geom = sum_geom(p, e);       /* 2^(e+1)-1   */
            seq_t sub  = pow_seq_uint(p, e / 2);/* 2^(e/2)     */
            S = geom - sub;
        } else {                               /* general case */
            S = sum_geom(p, e);
        }

        result *= S;
    }

    return result;
}
