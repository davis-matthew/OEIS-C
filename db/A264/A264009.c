/*
 * A264009  Table T(i,j) = nonnegative k at which lcm(i+k,j+k) reaches the
 *          minimum, read by antidiagonals (i>=1, j>=1).
 * https://oeis.org/A264009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the pair read from the * table
 *            T(i,j) in antidiagonal order, and * T(i,j) = smallest
 *            non‑negative k such that * lcm(i+k , j+k) is minimal over all
 *            k≥0. * * The code maps n to (i,j), then scans k=0..|i-j| and *
 *            selects the k giving the minimal lcm
 * Range:     u8 n <= 91, u16 n <= 8001, u32 n <= 8001, u64 n <= 8001,
 *            u128 n <= 8001, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal number d such that
     * tot(d) = d*(d-1)/2 >= n.  We use a simple
     * incremental search; for n up to 10^9
     * this takes < 1000 iterations. */
    idx_t d = 1;
    while ((unsigned __int128)d * (d - 1) / 2 < n) {
        d++;
    }

    /* Number of terms before diagonal d. */
    idx_t prev = (idx_t)((unsigned __int128)(d - 1) * (d - 2) / 2);

    /* Zero‑based index on this diagonal. */
    idx_t pos = n - prev - 1;              /* 0 <= pos < d-1 */

    /* Coordinates (i,j) on the diagonal. */
    idx_t i = pos + 1;
    idx_t j = d - i;                       /* i,j ≥ 1 */

    /* Ensure i <= j to simplify |i-j|. */
    if (i > j) {
        idx_t tmp = i;
        i = j;
        j = tmp;
    }

    /* The search range for k is 0 … (j-i). */
    idx_t diff = j - i;

    seq_t best_k = 0;
    seq_t min_lcm = 0;
    seq_t ai = (seq_t)i;
    seq_t aj = (seq_t)j;

    for (idx_t k = 0; k <= diff; ++k) {
        seq_t a_val = ai + (seq_t)k;
        seq_t b_val = aj + (seq_t)k;
        seq_t g = gcd(a_val, b_val);
        seq_t lcm = (a_val / g) * b_val;

        if (k == 0 || lcm < min_lcm) {
            min_lcm = lcm;
            best_k  = (seq_t)k;
        }
    }

    return best_k;
}
