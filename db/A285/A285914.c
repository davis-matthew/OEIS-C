/*
 * A285914  Irregular triangle read by rows: T(n,k), n>=1, k>=1, in which
 *          column k lists k's interleaved with k-1 zeros, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A285914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = k*A237048(n,k); A237048(r,k)=1 iff r>=k(k+1)/2 and k
 *            divides r-k(k+1)/2; flattened by rows, row r has the k with
 *            k(k+1)/2<=r
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Validated: partial, pass, 112 terms through n = 112 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t row = 0, len = 0, next = 1, cum = 0;
    if (n == 0) return (seq_t)0;
    while (cum < n) {
        row++;
        if (row >= next) {
            len++;
            next = (len + 1) * (len + 2) / 2;
        }
        cum += len;
    }
    idx_t col = n - (cum - len);
    idx_t start = col * (col + 1) / 2;
    if (row < start) return (seq_t)0;
    if ((row - start) % col == 0) return (seq_t)col;
    return (seq_t)0;
}
