/*
 * A377917  Number of n-digit terms in A377912.
 * https://oeis.org/A377917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1)+15*a(n-2)+20*a(n-3)+15*a(n-4)+6*a(n-5)+a(n-6)
 *            for n>=8. Initial values: a(1)=10, a(2)=66, a(3)=489, a(4)=3631,
 *            a(5)=26951, a(6)=200045, a(7)=1484850
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms.  The cast to seq_t suppresses compiler warnings when
       seq_t is smaller than int.  Only the first seven terms are needed
       explicitly. */
    static const seq_t init[7] = {
        (seq_t)10,          /* a(1) */
        (seq_t)66,          /* a(2) */
        (seq_t)489,         /* a(3) */
        (seq_t)3631,        /* a(4) */
        (seq_t)26951,       /* a(5) */
        (seq_t)200045,      /* a(6) */
        (seq_t)1484850      /* a(7) */
    };

    /* Handle small n directly. */
    if (n == 0) {
        /* a(0) is undefined for the sequence; return 0. */
        return (seq_t)0;
    }
    if (n <= 7) {
        return init[n - 1];
    }

    /* Use a circular buffer of the last six values needed by the recurrence. */
    seq_t buf[6];
    /* Initialise buf with a(2) through a(7) to align indices:
       buf[0] = a(n-7), buf[1] = a(n-6), ..., buf[5] = a(n-1). */
    buf[0] = init[0];  /* a(1) */
    buf[1] = init[1];  /* a(2) */
    buf[2] = init[2];  /* a(3) */
    buf[3] = init[3];  /* a(4) */
    buf[4] = init[4];  /* a(5) */
    buf[5] = init[5];  /* a(6) */

    seq_t result = init[6]; /* a(7) */

    /* Compute values from n=8 up to the requested n. */
    for (idx_t k = 8; k <= n; ++k) {
        /* recurrence: a(k) = 5*a(k-1)+15*a(k-2)+20*a(k-3)
           +15*a(k-4)+6*a(k-5)+a(k-6) */
        seq_t next = 5 * result
                   + 15 * buf[5]
                   + 20 * buf[4]
                   + 15 * buf[3]
                   + 6  * buf[2]
                   +    buf[1];

        /* slide the buffer: shift left, drop a(k-7), insert a(k-1). */
        for (int i = 0; i < 5; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[5] = result;   /* store a(k-1) for the next iteration */

        result = next;     /* a(k) becomes the new result for next round */
    }

    return result;
}
