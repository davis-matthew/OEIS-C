/*
 * A337977  Triangle T(n,m) = C(n-1,n-m)*Sum_{k=1..n}
 *          C(2*k-2,k-1)*C(n-m,m-k)/m, m>0, n>0, n>=m.
 * https://oeis.org/A337977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, m) where r is the row containing n in the triangle *
 *            T(n, m) = binom(n-1, m-1) * Sum_{k=1..m} binom(2*k-2, k-1) *
 *            binom(n-m, m-k) / m * The sequence is obtained by flattening the
 *            triangle row by row
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Find the smallest row r such that r*(r+1)/2 >= n */
static idx_t
find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = 2;
    /* Exponentially grow high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* locate row r and column m within that row */
    idx_t r = find_row(n);
    idx_t prev_sum = ((idx_t)(r - 1) * r) / 2;   /* sum of row lengths before r */
    idx_t m = n - prev_sum;   /* 1‑based column index */

    /* compute the sum part S = Σ_{k=1..m} C(2k-2,k-1) * C(r-m, m-k) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t term1 = binomial(2 * k - 2, k - 1);
        seq_t term2 = binomial(r - m, m - k);
        sum += term1 * term2;
    }

    seq_t binom_factor = binomial(r - 1, m - 1);
    seq_t result = binom_factor * sum / (seq_t)m;
    return result;
}
