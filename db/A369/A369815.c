/*
 * A369815  Expansion of 1/(1 - x^4 - x^7).
 * https://oeis.org/A369815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-7), with a(0)=1, a(1)=a(2)=a(3)=0, a(4)=1,
 *            a(5)=a(6)=0, a(7)=1
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* A rolling buffer that holds the last 8 values of the sequence.
       The recurrence only refers to positions n-4 and n-7, so a buffer
       of size 8 is sufficient to compute any term. */
    seq_t buf[8];
    /* Ensure the buffer contains zeros and the initial value a(0)=1. */
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = (seq_t)0;
    buf[0] = (seq_t)1;

    /* If the requested index is 0, return the cached first element. */
    if (n == 0)
        return buf[0];

    /* Compute successive terms up to n.  The value of a(k) is written
       into buf[k & 7] so that old entries are overwritten after they
       are no longer needed. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= (idx_t)4)
            val += buf[(i - 4) & 7];
        if (i >= (idx_t)7)
            val += buf[(i - 7) & 7];
        buf[i & 7] = val;
    }

    /* Return the value that was stored for index n. */
    return buf[n & 7];
}
