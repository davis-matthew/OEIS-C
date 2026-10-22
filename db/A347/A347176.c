/*
 * A347176  G.f.: Sum_{k>=1} (-1)^(k+1) * k * x^(k^2) / (1 - x^(k^2)).
 * https://oeis.org/A347176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(2^e) = 3 - 2^{floor(e/2)+1},
 *            a(p^e) = (p^{floor(e/2)+1}-1)/(p-1) for odd primes p
 * Range:     i8 n <= 5183, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* empty product */
    /* factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (p == 2)
        {
            uint64_t half = e / 2;            /* floor(e/2) */
            seq_t pow2 = (seq_t)1;
            /* compute 2^{half+1} */
            for (uint64_t j = 0; j < half + 1; ++j) pow2 *= (seq_t)2;
            term = (seq_t)3 - pow2;           /* 3 - 2^{half+1} */
        }
        else
        {
            uint64_t exp = e / 2 + 1;         /* floor(e/2)+1 */
            seq_t powp = (seq_t)1;
            /* compute p^{exp} */
            for (uint64_t j = 0; j < exp; ++j) powp *= (seq_t)p;
            term = (powp - (seq_t)1) / ((seq_t)p - (seq_t)1);
        }
        result *= term;
    }
    return result;
}
