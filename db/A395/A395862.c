/*
 * A395862  Triangle read by rows: T(n,k) = numerator(CF(n,k)), where CF(n,k)
 *          = (1/k!) * Sum_{j=0..k} (-1)^j * binomial(k,j) * (k/2-j)^n.
 * https://oeis.org/A395862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = numerator of CF(r,k) where * r and k are obtained from n
 *            by n = r(r+1)/2 + k, 0 <= k <= r * CF(r,k) = (1/k!) * Σ_{j=0..k}
 *            (-1)^j * C(k,j) * (k/2 - j)^r * The numerator is taken after
 *            reducing the fraction to lowest terms
 * Range:     u8 n <= 17, u16 n <= 26, u32 n <= 74, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binomial, factorial, gcd, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"     // binomial(idx_t n, idx_t k)
#include "../utils/factorial.h"    // factorial(idx_t n)
#include "../utils/seq_pow.h"      // seq_pow(seq_t base, idx_t exp)
#include "../utils/pow2.h"         // pow2(idx_t exp)
#include "../utils/gcd.h"          // gcd(seq_t a, seq_t b)

seq_t a(idx_t n)
{
    /* 1. Convert the flattened index n into a row r and column k.
       The relation n = r(r+1)/2 + k, 0 <= k <= r is inverted. */
    idx_t idx = n;
    idx_t r = (idx_t)((sqrt((double)idx * 8.0 + 1.0) - 1.0) / 2.0);
    /* Correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= idx) ++r;
    while (r * (r + 1) / 2 > idx) --r;
    idx_t k = idx - r * (r + 1) / 2;

    /* 2. Compute the integer S = Σ (-1)^j C(k,j) (k-2j)^r. */
    seq_t pos = 0;           /* sum of positive terms */
    seq_t neg = 0;           /* sum of negative terms */

    for (idx_t j = 0; j <= k; ++j)
    {
        /* Difference (k - 2j) may be negative. */
        int64_t diff = (int64_t)k - 2 * (int64_t)j;
        bool diff_neg = diff < 0;
        seq_t abs_diff = (seq_t)(diff_neg ? -diff : diff);

        /* The sign of (k/2 - j)^r is the sign of diff^r. */
        bool pow_neg = diff_neg && (r & 1);   /* diff negative and r odd -> negative */

        /* Term sign: (-1)^j * sign(diff^r) */
        bool term_negative = ((j & 1) ^ pow_neg);

        seq_t bin = binomial(k, j);
        seq_t pow_term = seq_pow(abs_diff, r);
        seq_t term = bin * pow_term;

        if (term_negative)
            neg += term;
        else
            pos += term;
    }

    /* 3. If the sum is zero, the numerator is zero. */
    if ((pos | neg) == 0)   /* either pos or neg could be non-zero but equal to cancel */
        return 0;

    /* 4. Compute the absolute value of S. */
    seq_t S_abs = (pos >= neg) ? (pos - neg) : (neg - pos);

    /* 5. Denominator is k! * 2^r. */
    seq_t denom = factorial(k) * pow2(r);

    /* 6. Reduce the fraction S_abs/denom to lowest terms. */
    seq_t g = gcd(S_abs, denom);
    return S_abs / g;
}
