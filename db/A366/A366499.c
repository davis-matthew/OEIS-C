/*
 * A366499  G.f. A(x) satisfies A(x) = 1 + x / ((1+x)^3*A(x)^2).
 * https://oeis.org/A366499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366499
   Formula: a(n) = (-1)^(n-1) * Σ_{k=0..n} binomial(n+2*k-1,n-k) * binomial(3*k-1,k)/(3*k-1),
   with a(0) = 1
   (Binomial arguments are taken as 0 when top < bottom, which yields 0 anyway.) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin1 = binomial(n + 2 * k - 1, n - k);
        seq_t bin2 = binomial(3 * k - 1, k);
        seq_t prod = bin1 * bin2;
        seq_t term = prod / (seq_t)(3 * k - 1);
        sum += term;
    }
    seq_t sign = ((n - 1) & 1) ? (seq_t)-1 : (seq_t)1;
    return sum * sign;
}
