/*
 * A338721  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists the odd numbers k times, and the first element of
 *          column k is in row k(k+1)/2.
 * https://oeis.org/A338721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) with * T(n,k) = 2 * floor((n – k*(k-1)/2) / k) – 1
 *            * For a given 1‑based index n the function finds the row r such
 *            that the * cumulative number of terms up to r is ≥ n, determines
 *            the column k of * the n‑th term, and returns T(r,k)
 * Range:     u8 n <= 1255, u16 n <= 8975, u32 n <= 8975, u64 n <= 8975,
 *            u128 n <= 8975, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    idx_t r = 1;
    idx_t cumulative = 0;
    idx_t kmax = 0;

    /* Find the row containing the n-th term. */
    while (1) {
        /* Kmax(r) = floor((sqrt(8*r+1)-1)/2). */
        kmax = (int_sqrt((idx_t)8 * r + 1) - 1) / 2;
        cumulative += kmax;
        if (cumulative >= n) {
            break;
        }
        ++r;
    }

    idx_t sum_before = cumulative - kmax;   /* terms before this row   */
    idx_t k = n - sum_before;               /* column index (1‑based)  */

    /* Compute T(r,k). */
    seq_t kseq = (seq_t)k;
    seq_t rseq = (seq_t)r;
    seq_t offset = kseq * (kseq - 1) / 2;
    seq_t m = (rseq - offset) / kseq;
    seq_t value = 2 * m - 1;
    return value;
}
