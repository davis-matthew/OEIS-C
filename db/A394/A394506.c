/*
 * A394506  Triangle read by rows: T(n,k) = numerator(binomial(1/n, k)) with 1
 *          <= k <= n.
 * https://oeis.org/A394506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac, tabl, changed
 * Formula:   a(n) = numerator( binomial(1/r, k) ) where * r = smallest
 *            integer with r*(r+1)/2 >= n, * k = n - r*(r-1)/2 (1‑based column
 *            index)
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 51, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"

static inline seq_t abs_seq(seq_t x) { return x < 0 ? -x : x; }

/* Convert a sequence index to row (r) and column (k) of the triangle */
static void index_to_rc(idx_t n, idx_t *r, idx_t *k)
{
    /* Find r such that r*(r+1)/2 >= n */
    double dr = sqrt(8.0 * (double)n + 1.0);
    idx_t rr = (idx_t)((dr - 1.0) * 0.5);
    while (rr * (rr + 1) / 2 < n) ++rr;
    *r = rr;
    *k = n - (rr - 1) * rr / 2;  /* 1‑based column index */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset range – not used */
    idx_t r, k;
    index_to_rc(n, &r, &k);

    seq_t num = 1;   /* numerator of binomial(1/r, 0) */
    seq_t den = 1;   /* denominator of binomial(1/r, 0) */

    for (idx_t j = 1; j <= k; ++j) {
        seq_t mult = (seq_t)1 - (seq_t)(j - 1) * (seq_t)r;  /* (1 - (j-1)*r) */
        num *= mult;
        den *= (seq_t)r * (seq_t)j;        /* n * j  (here n is r) */

        seq_t g = gcd(abs_seq(num), den); /* reduce fraction */
        num /= g;
        den /= g;
    }
    return num;
}
