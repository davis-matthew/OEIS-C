/*
 * A302113  a(n) = (4/(2*n-3))*(2*(n-1)*(2*n-1)*a(n-1) +
 *          (-1)^n*Product_{k=0..n-1} (2*k+1)) with a(0) = 0.
 * https://oeis.org/A302113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4/(2*n-3)) * ( 2*(n-1)*(2*n-1)*a(n-1) + (-1)^n *
 *            ∏_{k=0}^{n-1}(2*k+1) ), a(0) = 0, a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* a(0) is the first term. */
    if (n == 0) return (seq_t)0;

    seq_t prev   = (seq_t)0;   /* a(0) */
    seq_t prodOdd = (seq_t)1;  /* product over first i odd numbers */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update product to include the next odd integer (2*i-1). */
        prodOdd *= (seq_t)(2 * i - 1);

        if (i == 1) {
            /* a(1) = 4 is special because the denominator would be -1. */
            prev = (seq_t)4;
            continue;
        }

        /* Compute the multiplicative factor 2*(i-1)*(2*i-1). */
        seq_t factor = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(2 * i - 1);

        /* term = 2*(i-1)*(2*i-1)*a(i-1). */
        seq_t term = factor * prev;

        /* Denominator 2*i-3 (positive for i >= 2). */
        long long denom_ll = 2LL * (long long)i - 3;
        seq_t denom = (seq_t)denom_ll;

        seq_t numerator;
        if ((i & 1ULL) == 0) {
            /* Even i : add the product. */
            numerator = (term + prodOdd) * (seq_t)4;
        } else {
            /* Odd i (i >= 3) : subtract the product (term > prodOdd). */
            numerator = (term - prodOdd) * (seq_t)4;
        }

        prev = numerator / denom;
    }

    return prev;
}
