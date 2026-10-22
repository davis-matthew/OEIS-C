/*
 * A304236  Triangle T(n,k) = T(n-1,k) + 3*T(n-2,k-1) for k = 0..floor(n/2),
 *          with T(0,0) = 1, T(n,k) = 0 for n or k < 0, read by rows.
 * https://oeis.org/A304236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = T(row,k) where * T(n,k) = 3^k * binomial(n-k,k) * with
 *            rows of length floor(row/2)+1
 * Range:     u8 n <= 22, u16 n <= 76, u32 n <= 234, u64 n <= 830,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

extern seq_t binomial(idx_t n, idx_t k);

seq_t a(idx_t n)
{
    idx_t acc = 0;            /* total number of triangle entries seen so far */
    for (idx_t row = 0; ; ++row) {
        idx_t len = (row / 2) + 1;             /* entries in this row */
        if (acc + len > n) {                   /* target entry is in this row */
            idx_t k = n - acc;                 /* column within the row */
            seq_t bin = binomial(row - k, k);   /* binomial part */
            seq_t pow3 = 1;                    /* compute 3^k multiplicatively */
            for (idx_t i = 0; i < k; ++i)
                pow3 *= (seq_t)3;
            return bin * pow3;
        }
        acc += len;                          /* move to next row */
    }
}
