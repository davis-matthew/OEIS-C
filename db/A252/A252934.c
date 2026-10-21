/*
 * A252934  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{min(9,n-1)} Q_i * C(n - i + 2, 3) where Q = {8,
 *            2, 65, 271, 1013, 2340, 2849, 1331, 293, 20} and C(k,3) =
 *            k*(k-1)*(k-2)/6 (k≥3, else 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 151, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* coefficients of the numerator polynomial in the g.f. */
static const seq_t Q[10] = {
    8,          /* 8   */
    2,          /* 2   */
    65,         /* 65  */
    271,        /* 271 */
    1013,       /* 1013*/
    2340,       /* 2340*/
    2849,       /* 2849*/
    1331,       /* 1331*/
    293,        /* 293 */
    20          /* 20  */
};

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1).  For safety, return 0 for n==0. */
    if (n == 0) { return (seq_t)0; }

    seq_t result = 0;
    idx_t max_i = n - 1;
    if (max_i > 9) { max_i = 9; }

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t t = n - i + 2;   /* argument of C(t,3) */
        if (t < 3) { continue; }

        /* compute C(t,3) using 128‑bit intermediate to avoid overflow */
        unsigned __int128 comb = (unsigned __int128)t * (t - 1);
        comb = comb * (t - 2) / 6;

        /* multiply by coefficient Q[i] and add to result */
        unsigned __int128 term = comb * (unsigned __int128)Q[i];
        result += (seq_t)term;
    }
    return result;
}
