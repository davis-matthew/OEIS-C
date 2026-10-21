/*
 * A252526  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 8*a(n-6) for n > 10 initial values: a(1)=730,
 *            a(2)=337, a(3)=334, a(4)=426, a(5)=676, a(6)=964, a(7)=1344,
 *            a(8)=2312, a(9)=3336, a(10)=4800
 * Range:     u8 none, u16 n <= 15, u32 n <= 39, u64 n <= 87, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL, size_t */

seq_t a(idx_t n)
{
    /* Hard‑coded initial terms, index 0 unused to match OEIS offset 1 */
    static const seq_t init_vals[11] = {
        (seq_t)0,      /* a(0) – unused */
        (seq_t)730,    /* a(1) */
        (seq_t)337,    /* a(2) */
        (seq_t)334,    /* a(3) */
        (seq_t)426,    /* a(4) */
        (seq_t)676,    /* a(5) */
        (seq_t)964,    /* a(6) */
        (seq_t)1344,   /* a(7) */
        (seq_t)2312,   /* a(8) */
        (seq_t)3336,   /* a(9) */
        (seq_t)4800    /* a(10) */
    };

    /* For n within the pre‑given range, return the stored value. */
    if (n <= 10)
        return init_vals[n];

    /* Ring buffer holding the latest six values needed for the recurrence.
       Buffer index mapping: buf[i%6] holds a(i+1).  Thus buf[(i-1)%6] holds a(i). */
    seq_t buf[6];

    /* Initialise the buffer with the first ten values. */
    for (idx_t i = 1; i <= 10; ++i)
        buf[(i - 1) % 6] = init_vals[i];

    /* Compute terms from 11 up to n using the recursion. */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = buf[(i - 3 - 1) % 6] * (seq_t)6
                    - buf[(i - 6 - 1) % 6] * (seq_t)8;
        buf[(i - 1) % 6] = val;
    }

    /* The desired term a(n) is now stored at buf[(n-1)%6]. */
    return buf[(n - 1) % 6];
}
