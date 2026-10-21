/*
 * A265710  a(n) = denominator of Sum_{d|n} 1/sigma(d).
 * https://oeis.org/A265710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} 1/sigma(d)
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(k) = sum of divisors of k. */
/* This implementation pre‑computes sigma values up to the largest n seen
   and uses a fraction addition routine for each divisor. */

#include <stddef.h>
#include "../utils/gcd.h"

/* Small primes up to 100 (enough for any n <= 10 000). */
static const idx_t _primes_small[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97
};
static const idx_t _primes_small_cnt =
    sizeof(_primes_small)/sizeof(_primes_small[0]);

/* Compute sigma(m) by prime factorisation.  Works for m up to 10 000. */
static seq_t _sigma_uint(idx_t m)
{
    if (m == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    idx_t temp = m;

    for (idx_t i = 0; i < _primes_small_cnt; ++i) {
        idx_t p = _primes_small[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Compute p^(e+1) */
            seq_t ppow = (seq_t)1;
            for (idx_t j = 0; j < e + 1; ++j) ppow *= (seq_t)p;
            /* term = (p^(e+1)-1)/(p-1) */
            seq_t term = (ppow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            result *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t term = (seq_t)temp + (seq_t)1;
        result *= term;
    }
    return result;
}

/* Fraction addition: updated_num/updated_den = old_num/old_den + 1/x,
   where old_num/old_den is reduced.  The result is reduced. */
static void _add_reciprocal(seq_t *num, seq_t *den, seq_t x)
{
    seq_t num_new = (*num) * x + (*den);
    seq_t den_new = (*den) * x;
    seq_t g = gcd(num_new, den_new);
    *num = num_new / g;
    *den = den_new / g;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t num = (seq_t)0; /* numerator of the running sum */
    seq_t den = (seq_t)1; /* denominator of the running sum */

    /* Enumerate divisors of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t sigma1 = _sigma_uint(d);
            _add_reciprocal(&num, &den, sigma1);

            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t sigma2 = _sigma_uint(d2);
                _add_reciprocal(&num, &den, sigma2);
            }
        }
    }
    return den; /* denominator of Sum_{d|n} 1/sigma(d) */
}
