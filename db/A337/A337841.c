/*
 * A337841  Triangle read by rows, T(n, k) = binomial(2*n-1, 2*k-1) *
 *          binomial(2*n-2*k, n-k) * (k+1) / binomial(n+k+1, n-k) for 0 <= k
 *          <= n.
 * https://oeis.org/A337841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = flattening of the triangle T, where * T(r,k) =
 *            binomial(2*r-1, 2*k-1) * binomial(2*r-2*k, r-k) * * (k+1) /
 *            binomial(r+k+1, r-k) (0≤k≤r) * with T(r,0) = 0^r (1 for r=0).
 *            Index n is row‑major 0‑based
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */

/* compute the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
static inline idx_t row_from_index(idx_t n)
{
    idx_t r = 0;
    /* simple O(√n) search – fast enough for n≈10000 */
    while (true) {
        __int128 next = (__int128)(r + 1) * (r + 2) / 2;
        if (next > n) break;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* determine (row, column) from the flat index */
    idx_t row = row_from_index(n);
    idx_t col = n - (row * (row + 1)) / 2;     /* 0‑based column */

    /* special value T(row,0) */
    if (col == 0) return (seq_t)(row == 0 ? 1 : 0);

    /* use the closed‑form formula for T(row,col) */
    seq_t part1 = binom(2 * row - 1, 2 * col - 1);
    seq_t part2 = binom(2 * row - 2 * col, row - col);
    seq_t denom  = binom(row + col + 1, row - col);

    seq_t num = part1 * part2;
    num = num * (seq_t)(col + 1);
    seq_t result = num / denom;   /* exact division as defined */

    return result;
}
