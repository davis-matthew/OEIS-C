/*
 * A374356  a(n) is the greatest fibbinary number f <= n such that n - f is
 *          also a fibbinary number whose binary expansion has no common 1's
 *          with that of f (where fibbinary numbers correspond to A003714).
 * https://oeis.org/A374356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = keep every other bit in each run of consecutive 1's
 *              of the binary expansion of n, starting with the most
 *              significant 1 of that run.
 * This is equivalent to: a(n) is the greatest fibbinary number f ≤ n
 * such that (n - f) is also fibbinary and f & (n - f) == 0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find most significant set bit of n (0‑based). */
    int msb = 63;
    while ((n >> msb) == 0)
        --msb;

    idx_t f = 0;          /* result in 64‑bit form before cast */
    int keep = 0;         /* whether to keep the current 1bit */
    int in_run = 0;       /* whether we are inside a run of 1's */

    for (int bit = msb; bit >= 0; --bit)
    {
        if ((n >> bit) & 1U)         /* current bit of n is 1 */
        {
            if (!in_run)             /* start of a new run */
            {
                in_run = 1;
                keep   = 1;          /* keep the first (most significant) bit of the run */
            }
            else                     /* continuation of the run */
                keep ^= 1;           /* toggle keep for every alternating bit */

            if (keep)
                f |= ((idx_t)1U << bit);
        }
        else                          /* current bit is 0 */
        {
            in_run = 0;               /* run ends */
        }
    }

    return (seq_t)f;
}
