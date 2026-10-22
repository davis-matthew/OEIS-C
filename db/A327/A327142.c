/*
 * A327142  a(n) is the number of different sizes of integer-sided rectangles
 *          which can be placed inside an n X n square and with length greater
 *          than n.
 * https://oeis.org/A327142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n*sqrt(2)) - n, 2)
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts, for a given n, the number of distinct rectangle
 * sizes with one side longer than n that can fit inside an n×n square.
 * Calculation is done with integer arithmetic only.
 *
 * floor(n*sqrt(2)) is obtained as floor(sqrt(2*n^2)) by the integer
 * square‑root routine provided by the utility library.
 *
 * The binomial is computed as m*(m-1)/2, with the division by two
 * performed first to avoid overflow for the usual range of n. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */

    seq_t n_seq = (seq_t)n;

    /* m2 = 2 * n^2  (compute in seq_t to avoid overflow for small n) */
    seq_t m2 = (seq_t)2 * n_seq * n_seq;

    /* floor of sqrt(2)*n */
    seq_t t = floor_sqrt(m2);

    /* m = floor(n*sqrt(2)) - n */
    seq_t m = (t > n_seq) ? (t - n_seq) : (seq_t)0;

    if (m < (seq_t)2) return 0;      /* binomial is zero when m<2 */

    /* compute m*(m-1)/2 safely */
    seq_t a_val;
    if ((m & 1) == 0) {              /* m is even */
        a_val = (m >> 1) * (m - 1);
    } else {                         /* m-1 is even */
        a_val = ((m - 1) >> 1) * m;
    }

    return a_val;
}
