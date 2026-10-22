/*
 * A346527  a(n) is the largest number such that there are precisely n squares
 *          between a(n) and 2*a(n) inclusive.
 * https://oeis.org/A346527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the largest integer k such that exactly n perfect
 *            squares lie in [k, 2k] (inclusive)
 * Range:     u8 n <= 5, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the largest k for a given n. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t best = 0;

    /* Upper bound for a = floor(sqrt(k-1)).
       a grows roughly as 2.5 * n, so 3*n+5 is safely above the maximum. */
    idx_t a_start = 3 * (n + 1);
    for (idx_t a = a_start + 1; a-- > 0; )
    {
        seq_t a_seq = (seq_t)a;
        seq_t a_sq  = a_seq * a_seq;

        /* Interval where floor(sqrt(k-1)) == a. */
        seq_t lower1 = a_sq + 1;                 /* k > a^2              */
        seq_t upper1 = (a_seq + 1) * (a_seq + 1);/* k <= (a+1)^2          */

        seq_t b_seq = a_seq + n_seq;             /* required floor(sqrt(2k)) */
        seq_t b_sq  = b_seq * b_seq;

        /* Interval where floor(sqrt(2k)) == b. */
        seq_t lower2 = (b_sq + 1) / 2;              /* ceil(b^2 / 2)          */
        seq_t upper2 = ((b_seq + 1) * (b_seq + 1) - 1) / 2; /* floor(((b+1)^2-1)/2) */

        /* Overlap of the two intervals. */
        seq_t low  = lower1 > lower2 ? lower1 : lower2;
        seq_t high = upper1 < upper2 ? upper1 : upper2;

        if (low <= high && high > best)
            best = high;
    }
    return best;
}
