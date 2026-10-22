/*
 * A392627  Tetrahedron read by slices read by antidiagonals: T(n,i,j) is the
 *          number of paths from (0,0,0) to (n,i,j) consisting of moves
 *          {(1,0,0), (1,1,0), (1,0,1), (1,1,1)} that remain confined to
 *          i+j<=n at each step.
 * https://oeis.org/A392627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the value T(s,i,j) read from the 3‑D tetrahedron * where
 *            slice s is the smallest integer with cumulative size * S(s) =
 *            (s+1)(s+2)(s+3)/6 > n, and the position (i,j) inside that slice
 *            * is determined by reading the slice antidiagonally (t = i+j), *
 *            with i increasing along each antidiagonal...
 * Range:     u8 n <= 95, u16 n <= 325, u32 n <= 1651, u64 n <= 8946,
 *            u128 n <= 9754, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient C(n,k) for 0 <= k <= n.   */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t d = 1; d <= k; ++d) {
        res = res * (seq_t)(n - k + d);
        res = res / (seq_t)d;
    }
    return res;
}

/* Main function as required by the build system. */
seq_t a(idx_t n)
{
    /* Find the slice s such that S(s-1) <= n < S(s). */
    idx_t s = 0;
    idx_t cum = 0;  /* cumulative elements before slice s */
    while (1) {
        idx_t slice_size = (idx_t)( (s + 1) * (s + 2) / 2 );
        if (cum + slice_size > n) break;
        cum += slice_size;
        ++s;
    }

    /* Offset inside the current slice. */
    idx_t offset = n - cum;  /* 0 <= offset < slice size */

    /* Find antidiagonal t with cumulative size up to t > offset. */
    idx_t t = 0;
    while ( ( (t + 1) * (t + 2) / 2 ) <= offset ) ++t;

    /* Position inside antidiagonal. */
    idx_t prev_cum = (idx_t)(t * (t + 1) / 2);
    idx_t d = offset - prev_cum;   /* d = i */
    idx_t i = d;
    idx_t j = t - d;

    /* Compute T(s,i,j). */
    seq_t ci = binom_idx(s, i);
    seq_t cj = binom_idx(s, j);
    seq_t term = ci * cj;
    if (i > 0 && j > 0) {
        seq_t cim1 = binom_idx(s, i - 1);
        seq_t cjm1 = binom_idx(s, j - 1);
        term -= cim1 * cjm1;
    }
    return term;
}
