/*
 * A274723  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) or (-1,0) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A274723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) - 3*a(n-4) - 4*a(n-5) for n
 *            > 7
 *            a(1)=5,a(2)=24,a(3)=68,a(4)=260,a(5)=1040,a(6)=4132,a(7)=16524
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1 .. 7 */
    static const seq_t base[8] = {
        0ULL,          /* 0 unused */
        (seq_t)5,      /* a(1) */
        (seq_t)24,     /* a(2) */
        (seq_t)68,     /* a(3) */
        (seq_t)260,    /* a(4) */
        (seq_t)1040,   /* a(5) */
        (seq_t)4132,   /* a(6) */
        (seq_t)16524   /* a(7) */
    };

    /* For small n return the stored values */
    if (n <= 7) {
        return base[n];
    }

    /* Sliding window of the last 5 terms:
       a[i-5], a[i-4], a[i-3], a[i-2], a[i-1]  */
    seq_t buf[5];
    buf[0] = base[3];   /* a(3) */
    buf[1] = base[4];   /* a(4) */
    buf[2] = base[5];   /* a(5) */
    buf[3] = base[6];   /* a(6) */
    buf[4] = base[7];   /* a(7) */

    idx_t i;
    for (i = 8; i <= n; ++i) {
        seq_t next = (seq_t)3 * buf[4]      /* 3*a(i-1) */
                    + (seq_t)4 * buf[3]      /* 4*a(i-2) */
                    +          buf[2]       /*   a(i-3) */
                    - (seq_t)3 * buf[1]      /* -3*a(i-4) */
                    - (seq_t)4 * buf[0];     /* -4*a(i-5) */

        /* shift window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    /* After loop, buf[4] contains a(n) */
    return buf[4];
}
