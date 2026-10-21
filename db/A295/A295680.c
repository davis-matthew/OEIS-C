/*
 * A295680  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 0, a(1) = 0, a(2) =
 *          0, a(3) = 2.
 * https://oeis.org/A295680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(n) = a(n-1) + a(n-3) + a(n-4)
 *            (n ≥ 4)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* handle the initial terms directly */
    if (n < 3)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)2;

    /* a small circular buffer holding the last four values.
       buf[i] always contains a(i) for the current i modulo 4. */
    seq_t buf[4];
    buf[0] = (seq_t)0;  /* a(0) */
    buf[1] = (seq_t)0;  /* a(1) */
    buf[2] = (seq_t)0;  /* a(2) */
    buf[3] = (seq_t)2;  /* a(3) */

    /* iterate from 4 up to n computing each term in turn */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = buf[(i - 1) & 3];   /* a(i-1) */
        val += buf[(i - 3) & 3];       /* a(i-3) */
        val += buf[(i - 4) & 3];       /* a(i-4) */
        buf[i & 3] = val;              /* store a(i) */
    }

    return buf[n & 3];
}
