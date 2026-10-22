/*
 * A333560  Square array read by antidiagonals: T(n,k) = Sum_{j = 0..n*k}
 *          binomial(n+j-1,j)*2^j; n,k >= 0.
 * https://oeis.org/A333560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = T(n,k) read by antidiagonals of T(n,k) = Sum_{j=0}^{n*k}
 *            binomial(n+j-1,j)*2^j
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 47, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *k)
{
    /* Find the smallest d such that (d+1)(d+2)/2 > n.
       Using unsigned __int128 to avoid overflow when d becomes large. */
    idx_t d = 0;
    while ( ((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n )
        ++d;

    idx_t off = n - (d * (d + 1)) / 2;
    *i = d - off;   /* row index (n) */
    *k = off;       /* column index (k) */
}

static inline seq_t T_row_col(idx_t i, idx_t k)
{
    idx_t m = i * k;          /* upper bound for j */
    seq_t sum  = 0;
    seq_t C    = 1;           /* binomial(i + j - 1, j) when j = 0 */
    seq_t pow2 = 1;           /* 2^j for j = 0 */

    for (idx_t j = 0; j <= m; ++j) {
        sum += C * pow2;

        if (j == m) break;

        seq_t mult = (seq_t)(i + j);
        C = (C * mult) / (j + 1);
        pow2 <<= 1;
    }

    return sum;
}

seq_t a(idx_t n)
{
    idx_t i, k;
    antidiagonal_coords(n, &i, &k);
    return T_row_col(i, k);
}
