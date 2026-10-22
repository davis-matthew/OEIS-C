/*
 * A331762  Triangle read by rows: T(n,k) (1 <= k <= n) = Sum_{i=1..n, j=1..k,
 *          gcd(i,j)=2} (n+1-i)*(k+1-j).
 * https://oeis.org/A331762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{i=1..r} Sum_{j=1..k, gcd(i,j)=2} (r+1-i)*(k+1-j)
 *            where r is the row and k is the column obtained from the
 *            flattened triangle index n (rows are read row by row)
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the nth term of the triangle read by rows, indexed from 1. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 0;
    while ((__int128)r * (r + 1) / 2 < n)
        r++;

    /* Previous cumulative terms before this row */
    idx_t prev = (r - 1) * r / 2;
    /* Column index within the row (1-based) */
    idx_t k = n - prev;

    seq_t sum = 0;

    /* Sum over all i and j satisfying gcd(i,j)=2 */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t ni = (seq_t)(r + 1 - i);   /* (r+1-i) cast to seq_t */
        for (idx_t j = 1; j <= k; ++j) {
            if (gcd((seq_t)i, (seq_t)j) != 2)
                continue;
            seq_t kj = (seq_t)(k + 1 - j);   /* (k+1-j) cast to seq_t */
            sum += ni * kj;
        }
    }
    return sum;
}
