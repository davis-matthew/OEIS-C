/*
 * A391442  Irregular triangle read by rows: row n is the coefficients of the
 *          polynomial P_n such that A100976(2^n*k) = sigma(k) * P_n(2^k) for
 *          odd k, where sigma = A000203.
 * https://oeis.org/A391442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) is obtained by reading row‑wise the irregular triangle *
 *            defined by * * T(r,i) = -2^(2r+1-j) + 3*2^(2*(r-j)) if i = 2^r -
 *            2^j (0 ≤ j ≤ r) * T(r,i) = 0 otherwise. * * The sequence a(n)
 *            lists T(r,i) for rows r = 0,1,2,… and * indices i = 0,…,2^r-1 in
 *            lexicographic (row‑major) order. * * Implem...
 * Range:     i8 n <= 29, i16 n <= 509, i32 n <= 8395, i64 n <= 8395,
 *            i128 n <= 8395, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row r:  indices up to row r-1 total 2^r-1 numbers. */
    idx_t r   = floor_log2_uint64(n + 1);          /* r ≥ 0,  r ≤ 13 for n ≤ 10000 */
    idx_t start = ((idx_t)1 << r) - 1;             /* first index in row r   */
    idx_t pos   = n - start;                      /* position within row r  */

    /* i must be 2^r - 2^j.  Compute diff = 2^r - pos. */
    idx_t diff = ((idx_t)1 << r) - pos;           /* diff > 0  */
    if ((diff & (diff - 1)) != 0)                 /* not a power of two → zero entry   */
        return (seq_t)0;

    /* j = log2(diff) */
    idx_t j = floor_log2_uint64(diff);

    /* Compute the value: T(r,i) = -2^(2r+1-j) + 3*2^(2*(r-j)) */
    seq_t term1 = pow2(2 * r + 1 - j);
    seq_t term2 = pow2(2 * (r - j));
    return -term1 + 3 * term2;
}
