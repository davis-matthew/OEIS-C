/*
 * A382287  Irregular triangle T(n,k), n >= 0, 0 <= k <= 2*n+1, read by rows,
 *          where T(n,k) = [x^k] (1-x)^(n+1) * Sum_{k=0..n} (k+1)^n * x^k.
 * https://oeis.org/A382287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) is the n‑th element of the irregular triangle T(r,k) where
 *            r = floor((sqrt(4n+1)-1)/2) and k = n − r(r+1), and T(r,k) =
 *            Σ_{j=0..r} (−1)^(k−j) · (j+1)^r · binomial(r+1, k−j)
 * Range:     i8 n <= 16, i16 n <= 36, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* required for bool */
#include <stdint.h>           /* for standard integer types */
#include "../utils/pow_int.h"
#include "../utils/binomial.h"

static idx_t
find_row(idx_t n)
{
    idx_t r = 0;
    /* Find the largest r such that r*(r+1) <= n < (r+1)*(r+2) */
    while (1) {
        __int128 next = (__int128)(r + 1) * (r + 2);
        if ((idx_t)next > n)
            break;
        ++r;
    }
    return r;
}

seq_t
a(idx_t n)
{
    /* Determine the row r and column k (offset) of the flattened
       triangle that contains a(n). */
    idx_t r = find_row(n);
    idx_t start = r * (r + 1);   /* first index of row r */
    idx_t k = n - start;         /* 0‑based offset within the row */

    seq_t sum = 0;
    for (idx_t j = 0; j <= r; ++j) {
        if (k < j)
            continue;                         /* binomial would be zero */
        idx_t diff = k - j;
        if (diff > r + 1)
            continue;                         /* binomial would be zero */

        seq_t term = pow_int((seq_t)(j + 1), r); /* (j+1)^r */
        term *= binomial(r + 1, diff);          /* * C(r+1, k-j) */
        if ((diff & 1U) != 0)                   /* sign = (−1)^(k−j) */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
