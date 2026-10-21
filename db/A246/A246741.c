/*
 * A246741  Number of length 4+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence of order 15:
   a(n) = 3*a(n-1) + 3*a(n-2) - 17*a(n-3) + 3*a(n-4) + 39*a(n-5)
          - 25*a(n-6) - 45*a(n-7) + 45*a(n-8) + 25*a(n-9)
          - 39*a(n-10) - 3*a(n-11) + 17*a(n-12) - 3*a(n-13)
          - 3*a(n-14) + a(n-15)
   Initial values (n = 1 … 15):
   2, 30, 1156, 7612, 66294, 276762, 1231304, 3780600,
   11549290, 28599190, 69230412, 147079860, 304811486,
   578865042, 1074611344
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If called with n < 1, behaviour is undefined –
       we simply return 0 to satisfy the compiler.          */
    if (n < 1) return (seq_t)0;

    /* Base values for 1 ≤ n ≤ 15.  These are the
       known initial terms of the sequence.             */
    static const seq_t init[15] = {
        (seq_t)2,          /* a(1) */
        (seq_t)30,         /* a(2) */
        (seq_t)1156,       /* a(3) */
        (seq_t)7612,       /* a(4) */
        (seq_t)66294,      /* a(5) */
        (seq_t)276762,     /* a(6) */
        (seq_t)1231304,    /* a(7) */
        (seq_t)3780600,    /* a(8) */
        (seq_t)11549290,   /* a(9) */
        (seq_t)28599190,   /* a(10) */
        (seq_t)69230412,   /* a(11) */
        (seq_t)147079860,  /* a(12) */
        (seq_t)304811486,  /* a(13) */
        (seq_t)578865042,  /* a(14) */
        (seq_t)1074611344  /* a(15) */
    };

    if (n <= 15)
        return init[n - 1];

    /* Coefficients of the recurrence, in order a(n-1) … a(n-15). */
    static const int coeff[15] = {
        3,  3, -17,  3, 39,
       -25, -45, 45, 25, -39,
       -3,  17, -3, -3, 1
    };

    /* Buffer holding the last 15 computed terms. */
    seq_t buf[15];
    int i;
    for (i = 0; i < 15; ++i)      /* copy init into buf – memcpy
                                     is unsafe for non‑POD types. */
        buf[i] = init[i];

    /* The index of a(n-15) in the circular buffer. */
    int start = 0;

    /* Compute successive terms until reaching a(n). */
    for (idx_t k = 16; k <= n; ++k) {
        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of absolute values of negative contributions */

        for (i = 0; i < 15; ++i) {
            /* a(k-1-i) is stored at buffer position
               (start + 14 - i) mod 15. */
            int idx = (start + 14 - i) % 15;
            seq_t val = buf[idx];
            int c = coeff[i];

            if (c >= 0) {
                pos += val * (seq_t)c;
            } else {
                neg += val * (seq_t)(-c);
            }
        }

        /* Since the sequence is positive, pos ≥ neg holds. */
        seq_t next = pos - neg;

        /* Store the new term and advance the circular buffer. */
        buf[start] = next;
        start = (start + 1) % 15;
    }

    /* The last computed term a(n) is located just before 'start'. */
    int last = (start + 14) % 15;
    return buf[last];
}
