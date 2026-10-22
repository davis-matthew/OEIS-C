/*
 * A318678  Sum of deficiencies of deficient divisors of n.
 * https://oeis.org/A318678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, 2d > σ(d)} (2d - σ(d))
 * Range:     u8 n <= 16, u16 n <= 4596, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ(d) = sum of the positive divisors of d) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Recursive helper that walks the prime factorisation of n.  At each
   level cur_d is the divisor built up so far and cur_sigma is the
   product of σ(p^e) values for the primes already processed.  When all
   primes have been processed (pos == pcnt) we have a complete divisor
   d = cur_d with σ(d) = cur_sigma.  If the divisor is deficient
   (σ(d) < 2d) the deficiency 2d-σ(d) is added to the sum. */
static seq_t _sum_def(size_t pos, seq_t cur_d, seq_t cur_sigma,
                      const uint64_t *primes,
                      const uint8_t *exps,
                      size_t pcnt)
{
    if (pos == pcnt) {
        /* 2*d - σ(d) is positive iff σ(d) < 2*d  */
        if (cur_sigma < cur_d * ((seq_t)2)) {
            return cur_d * ((seq_t)2) - cur_sigma;
        }
        return (seq_t)0;
    }

    uint64_t p = primes[pos];
    uint8_t emax = exps[pos];
    seq_t total = (seq_t)0;
    seq_t p_pow = (seq_t)1;

    for (uint8_t e = 0; e <= emax; ++e) {
        /* σ(p^e) = (p^(e+1)-1)/(p-1)  */
        seq_t sigma_p = (seq_pow((seq_t)p, (idx_t)(e + 1)) - (seq_t)1)
                        / ((seq_t)p - (seq_t)1);

        total += _sum_def(pos + 1, cur_d * p_pow,
                          cur_sigma * sigma_p,
                          primes, exps, pcnt);
        p_pow *= (seq_t)p;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not needed for n>=1, guard */

    /* prime factorisation of n by trial division */
    uint64_t primes[64];
    uint8_t exps[64];
    size_t pcnt = 0;

    uint64_t temp = (uint64_t)n;
    uint64_t p = 2;
    while (p <= temp / p) {
        if (temp % p == 0) {
            exps[pcnt] = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exps[pcnt];
            }
            primes[pcnt++] = p;
        }
        p = (p == 2) ? 3 : p + 2;
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    return _sum_def(0, (seq_t)1, (seq_t)1, primes, exps, pcnt);
}
