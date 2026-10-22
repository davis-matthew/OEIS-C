/*
 * A389430  Number of nodes on a hexagonal lattice where the side lengths
 *          increase in this pattern: (x, x, x, x, x, x) -> (x+1, x, x, x+1,
 *          x, x) -> (x+1, x, x+1, x, x+1, x) -> (x+1, x+1, x, x+1, x+1, x) ->
 *          (x+1, x+1, x+1, x+1, x+1, x+1).
 * https://oeis.org/A389430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 589, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 7, a(6) = 10, a(7) = 12
 *   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-5) + 2*a(n-6) - 2*a(n-7) + a(n-8)   (n >= 8)
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the first eight values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)7;
    if (n == 6) return (seq_t)10;
    if (n == 7) return (seq_t)12;

    /* ring buffer holding the last eight values a(n-8)..a(n-1) */
    seq_t buf[8];
    buf[0] = (seq_t)0;  /* a(0)   */
    buf[1] = (seq_t)1;  /* a(1)   */
    buf[2] = (seq_t)2;  /* a(2)   */
    buf[3] = (seq_t)3;  /* a(3)   */
    buf[4] = (seq_t)4;  /* a(4)   */
    buf[5] = (seq_t)7;  /* a(5)   */
    buf[6] = (seq_t)10; /* a(6)   */
    buf[7] = (seq_t)12; /* a(7)   */

    /* iterate from the first term that requires the recurrence (n = 8) to the requested n */
    size_t i;
    for (i = 8; i <= (size_t)n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t next = 2 * buf[7]          /* 2*a(i-1) */
                  - 2 * buf[6]          /* -2*a(i-2) */
                  + 2 * buf[5]          /* +2*a(i-3) */
                  - 2 * buf[3]          /* -2*a(i-5) */
                  + 2 * buf[2]          /* +2*a(i-6) */
                  - 2 * buf[1]          /* -2*a(i-7) */
                  +     buf[0];         /* +a(i-8) */

        /* shift the buffer left by one and store the new value at the end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = next;
    }

    return buf[7];
}
