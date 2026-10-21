/*
 * A260170  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000101 or 00010001.
 * https://oeis.org/A260170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 49, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260170: Number of (n+2)×3 binary arrays with each 3×3 subblock
 * having a clockwise perimeter pattern 00000001, 00000101, or 00010001.
 *
 * Empirical recurrence (valid for n>11):
 *   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) + 6*a(n-4)
 *          - 8*a(n-5) - 3*a(n-6) - 4*a(n-7) - 10*a(n-8)
 *          - 4*a(n-9) + 8*a(n-10)
 *
 * Initial values:
 *   a(1)  = 40
 *   a(2)  = 74
 *   a(3)  = 183
 *   a(4)  = 409
 *   a(5)  = 950
 *   a(6)  = 2221
 *   a(7)  = 5235
 *   a(8)  = 12063
 *   a(9)  = 28230
 *   a(10) = 65845
 *   a(11) = 153721
 *
 * The function below computes a(n) for any n ≥ 1 using the recurrence
 * and constant‑time memory.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 0 – return 0 */

    static const seq_t init[11] = {
        40,      /* a(1)  */
        74,      /* a(2)  */
        183,     /* a(3)  */
        409,     /* a(4)  */
        950,     /* a(5)  */
        2221,    /* a(6)  */
        5235,    /* a(7)  */
        12063,   /* a(8)  */
        28230,   /* a(9)  */
        65845,   /* a(10) */
        153721   /* a(11) */
    };

    if (n <= 11) return init[n - 1];

    seq_t buf[10]; /* buf[0] = a(k-10), …, buf[9] = a(k-1) for the current k */

    /* initialize buffer with a(2) … a(11) for the first iteration (k=12) */
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[i + 1];

    seq_t value = 0;
    for (idx_t k = 12; k <= n; ++k) {
        /* compute a(k) from previous 10 values */
        value = buf[9];                                   /* a(k-1) */
        value += 2 * buf[8] + 2 * buf[7] + 6 * buf[6]      /* a(k-2), a(k-3), a(k-4) */
                + 8 * buf[0];                              /* a(k-10) */
        seq_t sub = 8 * buf[5] + 3 * buf[4] + 4 * buf[3]
                    + 10 * buf[2] + 4 * buf[1];            /* a(k-5) … a(k-9) */
        value -= sub;

        /* shift buffer left and insert new value at the end */
        for (idx_t i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = value;
    }

    return value;
}
