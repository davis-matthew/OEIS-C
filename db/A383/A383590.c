/*
 * A383590  a(n) = A378762(A382499(n)).
 * https://oeis.org/A383590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) defined by the triangular block permutation of A383590. For
 *            each n ≥ 1, find the row r such that S(r‑1) < n ≤ S(r) with S(r)
 *            = 2r² – r. Let m = 2r – 1, k = n – S(r‑1). Then a(n) = S(r‑1) +
 *            P(r,k) where P(r,k) = 2m – 1 – k if k < m and k odd P(r,k) = m +
 *            1 – k if k < m and k even P(r,k) = 3m...
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool in C */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; return 0 */

    /* Locate the row r:  S(r) = 2*r*r – r  >= n */
    idx_t r = 1;
    while (true) {
        idx_t sr = 2 * r * r - r;
        if (sr >= n) break;
        r++;
    }

    /* base = S(r-1) = (r-1)*(2r-3) */
    idx_t r_minus1 = r - 1;
    idx_t base_idx = r_minus1 * (2 * r - 3);
    seq_t base = (seq_t)base_idx;

    /* position within the row */
    idx_t k = n - base_idx;            /* 1 ≤ k ≤ 4r-3 */
    seq_t kk = (seq_t)k;
    seq_t m  = (seq_t)(2 * r - 1);

    seq_t P;
    bool k_lt_m = k < (idx_t)m;
    bool k_odd  = (k & 1) != 0;

    if (k_lt_m) {
        if (k_odd) {
            P = 2 * m - 1 - kk;
        } else {
            P = m + 1 - kk;
        }
    } else { /* k ≥ m */
        if (k_odd) {
            P = 3 * m - 1 - kk;
        } else {
            P = -m + kk;              /* kk ≥ m guarantees non‑negative */
        }
    }

    return base + P;
}
