/*
 * A373717  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..floor(k*n/(2*k+1))} binomial(k * (n-2*j),j).
 * https://oeis.org/A373717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) with (n,k) determined by the antidiagonal index:
 *            T(n,k) = \sum_{j=0}^{\lfloor k n/(2k+1)\rfloor} \binom{k
 *            (n-2j)}{j} The sequence a is obtained by reading the square
 *            array T by antidiagonals (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),…
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Count of entries up to and including all pairs with sum i+j <= s.
   For s >= 0  C(s) = (s+1)(s+2)/2.  Use 128‑bit to avoid overflow. */
static inline idx_t triangular(idx_t s)
{
    __int128 v = (__int128)(s + 1) * (s + 2);
    return (idx_t)(v / 2);
}

/* Convert the non‑negative index n into the coordinates (row,column)
   of the square array read by antidiagonals.  */
static inline void index_to_rc(idx_t n, idx_t *row, idx_t *col)
{
    if (n == 0) {
        *row = 0;
        *col = 0;
        return;
    }

    idx_t lo = 0;
    /* hi = n+1 is guaranteed to satisfy triangular(hi) > n for all n>=1 */
    idx_t hi = n + 1;

    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (triangular(mid) <= n)
            lo = mid;
        else
            hi = mid;
    }

    idx_t sum = hi;                     /* (row + col) */
    idx_t prev = triangular(sum - 1);   /* number of entries before this diagonal */
    idx_t offset = n - prev;             /* position within the diagonal */
    *row = offset;
    *col = sum - offset;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_rc(n, &row, &col);

    /* Compute T(row, col) = \sum_{j} binomial(col*(row-2*j), j) */
    seq_t result = (seq_t)0;
    if (col == 0)                           /* shortcut for col == 0 */
        return (seq_t)1;

    idx_t j_max = (col * row) / (2 * col + 1);
    for (idx_t j = 0; j <= j_max; ++j) {
        idx_t arg = col * (row - 2 * j);
        result += binomial((idx_t)arg, j);
    }
    return result;
}
