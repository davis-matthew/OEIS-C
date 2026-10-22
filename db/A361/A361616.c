/*
 * A361616  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..n}
 *          binomial(n+(k-1)*(j+1),n-j)/j!.
 * https://oeis.org/A361616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) = Σ_{k=0..i} C(i,k) * (j*(k+1))_{i-k}
 *            (j*(k+1))_{m} denotes the falling factorial x(x+1)...(x+m-1)
 *            with x = j*(k+1). T(i,0)=1 for all i. The mapping from index n
 *            to coordinates (i,j) uses antidiagonal numbering: i = offset, j
 *            = m - offset where m satisfies (m+1)(m+2)/2 > n
 * Range:     u8 n <= 24, u16 n <= 41, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find diagonal index m and offset within that diagonal */
    idx_t m = (idx_t)((sqrtl((long double)(8ULL * n + 1)) - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((m + 1) * (m + 2) / 2 <= n) ++m;
    while (m * (m + 1) / 2 > n) --m;

    idx_t offset = n - (m * (m + 1)) / 2;   /* row index i */
    idx_t i = offset;                       /* i = row */
    idx_t j = m - offset;                   /* j = column */

    /* T(i,0) = 1 */
    if (j == 0) return (seq_t)1;

    /* Compute sum Σ_{k=0..i} C(i,k) * falling factorial */
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;   /* C(i,0) */

    for (idx_t k = 0; k <= i; ++k) {
        /* Length of falling factorial: i - k */
        idx_t len = i - k;
        seq_t prod = (seq_t)1;

        if (len > 0) {
            seq_t start = (seq_t)j * (k + 1);  /* x = j*(k+1) */
            for (idx_t t = 0; t < len; ++t) {
                prod *= (start + t);
            }
        }

        sum += binom * prod;

        /* Update binomial coefficient for next k */
        if (k < i) {
            seq_t num = i - k;      /* (i-k) */
            seq_t den = k + 1;      /* (k+1) */
            binom = (binom * num) / den;  /* exact division */
        }
    }

    return sum;
}
