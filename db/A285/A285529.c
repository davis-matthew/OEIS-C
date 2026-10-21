/*
 * A285529  Triangle read by rows: T(n,k) is the number of nodes of degree k
 *          counted over all simple labeled graphs on n nodes, n>=1,
 *          0<=k<=n-1.
 * https://oeis.org/A285529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row * C(row-1, col) * 2^(C(row-1,2)) where row = smallest
 *            integer s.t. s*(s+1)/2 >= n col = n - row*(row-1)/2 - 1 (0‑based
 *            within the row) This generates the triangle read by rows of
 *            T(n,k)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

/* Find the row index (1‑based) containing the flattened term n.
   Returns an idx_t row such that row*(row+1)/2 >= n. */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Co‑ordinates are 1‑based for row, 0‑based for col. */
    if (n == 0)
        return 0;  /* out of range */

    idx_t row = triangular_root(n);
    __int128 prev_tri = (__int128)row * (row - 1) / 2;
    idx_t col = (idx_t)(n - prev_tri - 1);

    seq_t result = (seq_t)row;
    result *= binom(row - 1, col);
    result *= pow2((idx_t)binom(row - 1, 2));

    return result;
}
