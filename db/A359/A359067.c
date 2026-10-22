/*
 * A359067  a(2*n) = Sum_{k=0..n-1} binomial(2*n,k) binomial(2*n-1-k, n-1-k).
 *          a(2*n+1) = (Sum_{k=0..n} binomial(2*n+1,k) binomial(2*n-k, n-k)) -
 *          binomial(2*n-1, n).
 * https://oeis.org/A359067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359067
 *   a(1) = 0
 *   a(2*n)   = Σ_{k=0}^{n-1} binomial(2*n,k) * binomial(2*n-1-k, n-1-k)
 *   a(2*n+1) = Σ_{k=0}^{n}   binomial(2*n+1,k) * binomial(2*n-k, n-k)
 *                     - binomial(2*n-1, n)
 *   (n ≥ 1)
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0, a(0) is undefined but return 0 */
        return (seq_t)0;

    if ((n & 1) == 0) {       /* even term: n = 2*m */
        idx_t m = n >> 1;
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t b1 = binomial(2 * m, k);
            seq_t b2 = binomial(2 * m - 1 - k, m - 1 - k);
            sum += b1 * b2;
        }
        return sum;
    } else {                  /* odd term: n = 2*m+1, m ≥ 1 */
        idx_t m = (n - 1) >> 1;
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t b1 = binomial(2 * m + 1, k);
            seq_t b2 = binomial(2 * m - k, m - k);
            sum += b1 * b2;
        }
        seq_t sub = binomial(2 * m - 1, m);
        return sum - sub;
    }
}
