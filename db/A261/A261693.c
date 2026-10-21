/*
 * A261693  Irregular triangle read by rows in which row n lists the positive
 *          odd numbers in decreasing order starting with 2^n - 1. T(0, 1) = 0
 *          and T(n, k) for n >= 1, 1 <= k <= 2^(n-1).
 * https://oeis.org/A261693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(0) = 0. For n ≥ 1, let r = ⌊log₂(n)⌋ + 1; let k = n – 2^(r−1)
 *            + 1 (1‑based position in row r). Then a(n) = 2^r + 1 – 2*k
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   floor_log2_uint64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned r = floor_log2_uint64(n) + 1;              /* r ≥ 1 */
    idx_t start = ((idx_t)1) << (r - 1);                 /* 2^(r-1)  */
    idx_t k = n - start + 1;                            /* 1‑based index in row r */

    seq_t pow_r = seq_pow((seq_t)2, r);                   /* 2^r   */
    seq_t val = pow_r + (seq_t)1 - ((seq_t)2 * (seq_t)k);
    return val;
}
