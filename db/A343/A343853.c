/*
 * A343853  Irregular triangle read by rows: the n-th row gives the row
 *          indices of the matrix of 1..n^2 filled successively back and forth
 *          along antidiagonals.
 * https://oeis.org/A343853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the row index of the n‑th element in the antidiagonal
 *            back‑and‑forth filling of an r×r matrix, where r satisfies
 *            ∑_{k=1}^{r‑1} k² < n ≤ ∑_{k=1}^{r} k²
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    /* Find the row length r such that the n‑th term lies in the r×r matrix. */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (1) {
        r++;
        cumulative += r * r;
        if (n <= cumulative) {
            break;
        }
    }

    idx_t prev_cumulative = cumulative - r * r;
    idx_t pos = n - prev_cumulative;          /* 1‑based position inside the r×r matrix */

    /* Determine the row index of the pos‑th element in the antidiagonal
       traversal that alternates direction on successive diagonals. */
    idx_t remaining = pos;

    for (idx_t s = 2; s <= 2 * r; ++s) {
        idx_t len = (s <= r + 1) ? (s - 1) : (2 * r - s + 1);

        if (remaining <= len) {
            idx_t min_i = (s <= r + 1) ? 1 : (s - r);
            idx_t max_i = (s <= r + 1) ? (s - 1) : r;

            idx_t offset = remaining - 1;                  /* 0‑based within this diagonal */
            idx_t i = ((s & 1) == 1)                       /* odd diagonal forwards */
                          ? (min_i + offset)
                          : (max_i - offset);
            return (seq_t)i;
        }
        remaining -= len;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
