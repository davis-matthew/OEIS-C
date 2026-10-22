/*
 * A399227  a(n) = (2*n-1)*2^(w(2*n-1)-1) where w(m) = v_2(usigma(m)) =
 *          A007814(A034448(m)) for odd m.
 * https://oeis.org/A399227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1) * 2^(w(2*n-1)-1), where w(m) = v_2(usigma(m)) for
 *            odd m
 * Range:     u8 n <= 10, u16 n <= 190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2)                    /* values below the offset are irrelevant */
        return 0;

    /* m is the odd integer we must analyse */
    uint64_t m = (uint64_t)2 * n - 1;

    /* factorise m; the factors are small, a static buffer is sufficient */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* w(m) is the exponent of 2 in usigma(m) */
    unsigned w = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* usigma term for prime power p^e : 1 + p^e  */
        seq_t term = 1 + pow_uint(primes[i], exps[i]);

        /* add the 2–adic valuation of this term to w */
        seq_t t = term;
        while ((t & 1) == 0) {
            ++w;
            t >>= 1;
        }
    }

    /* 2^(w-1) */
    seq_t pow2 = pow_uint(2, w - 1);

    /* final value: (2*n-1) * 2^(w-1) */
    return (seq_t)m * pow2;
}
