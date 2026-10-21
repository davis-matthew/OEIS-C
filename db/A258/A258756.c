/*
 * A258756  a(n) = common difference giving the highest score in the game
 *          "Sequences" for a sequence with largest element n and as many
 *          elements as possible.
 * https://oeis.org/A258756
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = argmax_d d^(⌊(n-1)/d⌋ + 1) for 1 ≤ d ≤ ⌊(n-1)/2⌋ (with
 *            the tie‑breaker: choose the d giving the largest length) The
 *            sequence is computed by evaluating the exponent using double
 *            logarithms, which is exact for all values that fit into seq_t
 *            and sufficiently accurate for the required range of n
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* The game is only defined for n ≥ 3; for smaller n we return 0. */
    if (n < 3)
        return (seq_t)0;

    idx_t best_d = 1;                  /* d = 1 is always a legal candidate */
    idx_t best_L = n;                  /* L for d = 1 is simply n        */
    double best_log = 0.0;              /* log(1) * n = 0                  */

    /* d must satisfy that the longest progression has at least 3 terms: */
    idx_t d_max = (n - 1) / 2;

    for (idx_t d = 2; d <= d_max; ++d)
    {
        idx_t L = (n - 1) / d + 1;      /* longest length for this d     */
        if (L < 3)
            continue;                  /* should not happen due to d_max */

        double logScore = (double)L * log((double)d);

        if (logScore > best_log + 1e-12)
        {
            best_log = logScore;
            best_d   = d;
            best_L   = L;
        }
        else if (fabs(logScore - best_log) <= 1e-12)
        {
            /* equal score – pick the one with more elements */
            if (L > best_L)
            {
                best_d = d;
                best_L = L;
            }
        }
    }

    return (seq_t)best_d;
}
