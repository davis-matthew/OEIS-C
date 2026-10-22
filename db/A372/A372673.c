/*
 * A372673  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = phi(k*n) / phi(k).
 * https://oeis.org/A372673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the array T where T(n,k) = phi(k*n) /
 *            phi(k) read by antidiagonals downwards
 * Range:     u8 n <= 796, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;           /* undefined for offset < 1 */

    /* Find diagonal s (n + k = s + 1) that contains the n-th term.
       Diagonal lengths are s‑1, cumulative is triangular numbers. */
    idx_t s = 2;
    idx_t triangular = 1;          /* (s-1)*s/2 for s = 2 */
    while (triangular < n) {
        ++s;
        triangular += s - 1;        /* add next diagonal length */
    }

    idx_t prev_triangular = triangular - (s - 1);
    idx_t offset = n - prev_triangular - 1;   /* 0‑based within diagonal */
    idx_t row = offset + 1;       /* n‑index in T */
    idx_t col = s - row;          /* k‑index in T */

    seq_t prod = (seq_t)row * (seq_t)col;
    seq_t phi_prod = calc_phi(prod);
    seq_t phi_col  = calc_phi((seq_t)col);
    return phi_prod / phi_col;
}
