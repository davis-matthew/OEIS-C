/*
 * A295896  a(n) = 1 if there are no odd runs of 1's in the binary expansion
 *          of n followed by a 0 to their right, 0 otherwise.
 * https://oeis.org/A295896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 iff the binary expansion of n contains no odd run of
 *            consecutive 1's that is immediately followed by a 0 to its
 *            right; otherwise a(n) = 0
 * Range:     u8 n <= 9821, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t msb_idx(idx_t n)
{
    /* n > 0 is guaranteed by the caller */
    return 63 - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* by definition a(0) = 1 */

    int run = 0;
    int msb = (int)msb_idx(n);   /* msb index fits into int (0..63) */

    for (int i = msb; i >= 0; --i)
    {
        int bit = (n >> i) & 1U;

        if (bit)
        {
            ++run;               /* extend current run of 1's */
        }
        else /* bit == 0 */
        {
            if (run & 1)      /* odd run followed by 0 → invalid */
                return (seq_t)0;
            run = 0;           /* reset run counter */
        }
    }
    return (seq_t)1;               /* no invalid odd-run detected */
}
