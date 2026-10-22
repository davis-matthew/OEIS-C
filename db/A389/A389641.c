/*
 * A389641  Array read by ascending antidiagonals: A(n, k) =
 *          Sum_{j=0..floor(n/k)} n!/((j!)^k*(n - j*k)!) for k > 0 and A(n, 0)
 *          = 1.
 * https://oeis.org/A389641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(d-j, j) where d = ⌊(√(8n+1)-1)/2⌋ and j = n - d(d+1)/2.
 *            A(i,k) = Σ_{t=0..⌊i/k⌋} i! / ((t!)^k (i - t*k)!) (k>0), A(i,0) =
 *            1
 * Range:     u8 n <= 45, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/pow_uint.h"

/* Compute a single entry of the array A(i,k). */
static seq_t A_row_col(idx_t i, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    idx_t max_t = i / k;
    seq_t sum   = (seq_t)1;   /* term for t=0 */
    seq_t term  = (seq_t)1;   /* current term */

    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t numerator = (seq_t)1;
        for (idx_t r = 0; r < k; ++r) {
            numerator = numerator * (seq_t)(i - (t - 1) * k - r);
        }
        seq_t denom = pow_uint((uint64_t)t, k);   /* t^k */
        term = term * numerator / denom;
        sum += term;
    }
    return sum;
}

/* Public function returning the nth term of A389641. */
seq_t a(idx_t n)
{
    /* Locate the diagonal that contains position n.
       The number of terms before diagonal d is d(d+1)/2. */
    uint64_t d = (uint64_t)((sqrt((long double)8 * n + 1) - 1) / 2);

    uint64_t before = d * (d + 1) / 2;
    uint64_t j = n - before;        /* column index (k) */
    uint64_t i = d - j;              /* row index   */

    return A_row_col((idx_t)i, (idx_t)j);
}
