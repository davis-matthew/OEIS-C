/*
 * A352419  Triangle read by rows T(n,k): number of three-in-a-rows in
 *          n-dimensional tic-tac-toe through a cell that is central in k
 *          dimensions (for k=0..n).
 * https://oeis.org/A352419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (3^k - 1)/2 + 2^(n-k) - 1, where n and k are obtained by
 *            interpreting the sequence as a triangle read by rows. The
 *            sequence term a(idx) corresponds to the element in row n and
 *            column k of that triangle
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t idx)
{
    /* Find the row n such that n*(n+1)/2 <= idx < (n+1)*(n+2)/2.  */
    idx_t low = 0;
    idx_t high = 20000;  /* sufficient for n up to about 10000 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= idx)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t n = low;
    idx_t row_start = (n * (n + 1)) / 2;
    idx_t k = idx - row_start;   /* 0 <= k <= n */

    /* Compute the term using integer arithmetic. */
    seq_t p3 = pow3(k);               /* 3^k */
    seq_t part1 = (p3 - 1) / 2;       /* (3^k - 1)/2 */
    seq_t part2 = pow2(n - k);        /* 2^(n-k) */
    seq_t result = part1 + part2 - 1;
    return result;
}
