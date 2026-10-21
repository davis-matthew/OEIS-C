/*
 * A250862  Number of (n+2)X(2+2) 0..3 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(⌊(n+4)/3⌋+3,3) * C(⌊(n+3)/3⌋+3,3) *
 *            C(⌊(n+2)/3⌋+3,3))^2 · G(⌊(n+4)/3⌋) * G(⌊(n+3)/3⌋) *
 *            G(⌊(n+2)/3⌋), where G(m) is the number of pairs of weakly
 *            increasing 0..3 sequences of length m with component‑wise
 *            inequality
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 12, u128 n <= 63,
 *            big any n
 * Helpers:   count_pairs_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_pairs_m.h"

/* Compute binomial coefficient C(k,3) for k >= 3.  k is given as idx_t
   and the result is returned as seq_t. */
static seq_t binom3(idx_t k)
{
    seq_t kk = (seq_t)k;
    return (kk * (kk - 1) * (kk - 2)) / 6;
}

seq_t a(idx_t n)
{
    /* size of the array: (n+2) rows × 4 columns */
    idx_t L = n + 2;

    /* lengths of the three residue classes modulo 3 */
    idx_t m1 = (L + 2) / 3;   /* indices 1,4,7,...   */
    idx_t m2 = (L + 1) / 3;   /* indices 2,5,8,...   */
    idx_t m3 = L / 3;         /* indices 3,6,9,...   */

    /* g(L) = product of C(m_r+3,3) over r=1..3  (for a single column) */
    seq_t g1 = binom3(m1 + 3);
    seq_t g2 = binom3(m2 + 3);
    seq_t g3 = binom3(m3 + 3);
    seq_t g  = g1 * g2 * g3;          /* number for one column */

    /* full contribution of columns 2 and 3 (independent). */
    seq_t g2sq = g * g;              /* g(L)^2 */

    /* pairwise count for columns 1 and 4 with the row‑wise constraint. */
    seq_t G1 = count_pairs_m(m1);
    seq_t G2 = count_pairs_m(m2);
    seq_t G3 = count_pairs_m(m3);

    /* final result */
    seq_t res = g2sq * G1 * G2 * G3;
    return res;
}
