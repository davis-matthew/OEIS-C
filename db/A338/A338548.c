/*
 * A338548  a(n) = n^3 * Sum_{d|n} (-1)^(n/d + 1) * mu(d) / d^3.
 * https://oeis.org/A338548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n^3 * Σ_{d|n} (-1)^(n/d+1) * μ(d) / d^3 Using the
 *            multiplicative form: a(n) = J_3(n) if n is odd a(n) = J_3(n) -
 *            16 * J_3(n/2) if n is even where J_3(n) = n^3 ∏_{p|n} (1 -
 *            1/p^3) (Jordan's totient of order 3)
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1245, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static seq_t jordan3(idx_t n)
{
    /* Compute J_3(n) = ∏_{p^e||n} p^(3e-3) * (p^3 - 1)   */
    seq_t result = 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];          /* exponent of prime p in n */
        seq_t term = (seq_t)(p * p * p - 1);

        /* p^(3*e-3) may be zero for e=1, which is fine (seq_pow handles exp=0). */
        seq_t p_pow = seq_pow((seq_t)p, 3 * e - 3);

        result *= p_pow;
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t j3 = jordan3(n);
    if ((n & 1ULL) == 0) {          /* even n */
        seq_t j3_half = jordan3(n >> 1);   /* n/2  */
        return j3 - (j3_half * 16);
    } else {                       /* odd n */
        return j3;
    }
}
