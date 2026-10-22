/*
 * A372906  a(n) = Sum_{j=1..n} Sum_{k=1..n} gcd(j,n) * gcd(k,n) / gcd(j*k,n).
 * https://oeis.org/A372906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{j=1..n} Sum_{k=1..n} gcd(j,n)*gcd(k,n)/gcd(j*k,n)
 * Range:     u8 n <= 13, u16 n <= 203, u32 n <= 10000, u64 n <= 10000,
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
    if (n <= 1)
        return (seq_t)n;   /* a(1)=1, a(0)=0 (unused) */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;   /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p     = primes[i];
        unsigned int e = (unsigned int)exps[i];
        if (e == 0) continue;   /* should not happen */

        seq_t p_seq = (seq_t)p;

        /* precompute powers p^0 .. p^e */
        seq_t p_pow[64];
        p_pow[0] = 1;
        for (unsigned int t = 1; t <= e; ++t)
            p_pow[t] = p_pow[t - 1] * p_seq;

        seq_t val = 0;   /* value for this prime power */

        for (unsigned int a = 0; a <= e; ++a) {
            seq_t Ca = (a < e) ? (p_pow[e - a - 1] * ((seq_t)(p - 1))) : (seq_t)1;
            for (unsigned int b = 0; b <= e; ++b) {
                seq_t Cb = (b < e) ? (p_pow[e - b - 1] * ((seq_t)(p - 1))) : (seq_t)1;
                unsigned int exp = (a + b > e) ? (a + b - e) : 0;
                seq_t term = Ca * Cb * p_pow[exp];
                val += term;
            }
        }

        result *= val;
    }

    return result;
}
