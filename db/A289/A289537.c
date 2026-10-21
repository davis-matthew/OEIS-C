/*
 * A289537  Triangle read by rows: T(n,k) is the number of k-dimensional
 *          subspaces of an n-dimensional vector space over F_2 that do not
 *          contain a given nonzero vector, n>=0, 0<=k<=n.
 * https://oeis.org/A289537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   If n = r(r+1)/2 + k (0‑≤ k ≤ r) then a(n) = 2^k ·
 *            GaussianBinomial₂(r‑1, k) (Number of k‑dimensional subspaces of
 *            F₂^r not containing a fixed non‑zero vector)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 50, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* determine the row r of the triangle */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* column k within that row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* trivial cases */
    if (k == 0) return (seq_t)1;
    if (r == 0) return (seq_t)1;   /* only k==0 is possible here */

    /* compute Gaussian binomial coefficient C₂(r‑1, k) multiplicatively */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        seq_t term_num = pow2((r - 1) - i) - (seq_t)1;
        seq_t term_den = pow2(k - i)     - (seq_t)1;
        num *= term_num;
        den *= term_den;
    }

    seq_t g   = num / den;      /* exact division */
    seq_t res = g * pow2(k);    /* multiply by 2^k */

    return res;
}
