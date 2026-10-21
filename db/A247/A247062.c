/*
 * A247062  Dynamic Betting Game D(n,5,2).
 * https://oeis.org/A247062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-8) - a(n-9) for n ≥ 9, a(0) = 0 * The first
 *            eight differences (a(n)-a(n-1) for 0 ≤ n < 8) are * 1, 1, 3, 1,
 *            2, 3, 1, 4, which produces the initial values * a(1)=1, a(2)=2,
 *            a(3)=5, … as listed in the OEIS entry
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t */

static const seq_t diff[8] = { 1, 1, 3, 1, 2, 3, 1, 4 };

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(0) is used only in
       the recurrence.  If the caller passes 0 we simply return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* A fixed-size ring buffer holding the last 9 values:
       buf[0] = a(i-9), buf[1] = a(i-8), …, buf[8] = a(i-1) before
       computing a(i).  It is initialised with a(0)…a(8). */
    seq_t buf[9];
    buf[0] = (seq_t)0;                     /* a(0) */

    /* Initialise a(1)…a(8) using the period‑8 difference list. */
    for (idx_t i = 1; i <= 8; ++i)
        buf[i] = buf[i - 1] + diff[(i - 1) % 8] + 0;   /* +0 just keeps type */

    /* For n ≤ 8 the value is already in the buffer. */
    if (n <= (idx_t)8)
        return buf[n];

    /* Compute successive terms using the recurrence.
       For each i (starting at 9) we have:
           a(i) = a(i-1) + a(i-8) - a(i-9)
         which in terms of the buffer is:
           new_val = buf[8] + buf[1] - buf[0].
       After storing the new value we slide the window left by one. */
    for (idx_t i = 9; i <= n; ++i)
    {
        seq_t new_val = buf[8] + buf[1] - buf[0];

        /* Shift the window: buf[0] becomes a(i-8), etc. */
        for (idx_t j = 0; j < 8; ++j)
            buf[j] = buf[j + 1];
        buf[8] = new_val;
    }

    /* After the loop buf[8] contains a(n). */
    return buf[8];
}
