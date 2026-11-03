/*
 * A316595  a(n) equals the coefficient of x^n in Sum_{m>=0} (x^m + 5 +
 *          1/x^m)^m for n >= 1.
 * https://oeis.org/A316595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m|n} sum_{i=k}^{⌊(m+k)/2⌋} binomial(m,i) *
 *            binomial(m-i,i-k) * 5^(m-2i+k) , where k = n/m
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 257 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t calc_m(idx_t m, idx_t n)
{
    idx_t k = n / m;                     /* k = n/m, integer division holds because m|n */
    seq_t part = 0;
    /* i loops from k up to floor((m+k)/2), but never exceed m          */
    idx_t i_max = (m + k) / 2;
    if (i_max > m) i_max = m;
    for (idx_t i = k; i <= i_max; ++i) {
        /* binomial(m,i) is defined for 0 <= i <= m                      */
        seq_t bi = binomial(m, i);
        /* binomial(m-i, i-k) requires i-k <= m-i, which holds by i_max */
        seq_t b2 = binomial(m - i, i - k);
        idx_t exp = m - 2 * i + k;        /* exponent for 5^...                           */
        seq_t p5 = power5(exp);
        part += bi * b2 * p5;
    }
    return part;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1, but guard just in case */
    seq_t result = 0;
    /* iterate over all positive divisors m of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t m1 = d;
        idx_t m2 = n / d;
        result += calc_m(m1, n);
        if (m2 != m1) result += calc_m(m2, n);  /* avoid double counting when n is a square */
    }
    return result;
}
