/*
 * A305764  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+9*a(n-2)-6*a(n-4)+12*a(n-5)-3*a(n-6)-27*a(n-7)+17*a(n-8)-4*a(n-9)-7*a(n-10)+a(n-11)
 *            with a(1) = 4, a(2)=30, a(3)=141, a(4)=684, a(5)=3282,
 *            a(6)=15874, a(7)=76613, a(8)=370087, a(9)=1787161,
 *            a(10)=8631009, a(11)=41682223
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first 11 terms are hard‑coded. */
    static const seq_t init[12] = {
        0,            /* dummy for 0‑th index */
        4,            /* a(1) */
        30,           /* a(2) */
        141,          /* a(3) */
        684,          /* a(4) */
        3282,         /* a(5) */
        15874,        /* a(6) */
        76613,        /* a(7) */
        370087,       /* a(8) */
        1787161,      /* a(9) */
        8631009,      /* a(10) */
        41682223      /* a(11) */
    };

    if (n <= 11) return init[n];

    /* Buffer holding the last 11 values.
       buf[0] = a(n-11), buf[10] = a(n-1).  */
    seq_t buf[11];
    for (int j = 0; j < 11; ++j) buf[j] = init[j + 1];

    /* Compute terms from n = 12 up to the requested value. */
    for (idx_t i = 12; i <= n; ++i) {
        /* Build the next term from the recurrence, avoiding signed
           arithmetic with negative coefficients. */
        seq_t pos = buf[10] * 3 + buf[9] * 9 + buf[6] * 12 + buf[3] * 17 + buf[0];
        seq_t neg = buf[7] * 6 + buf[5] * 3 + buf[4] * 27 + buf[2] * 4 + buf[1] * 7;
        seq_t next = pos - neg;   /* the recurrence always yields a positive result */

        /* Shift the window: move all older values one step forward. */
        for (int j = 0; j < 10; ++j) buf[j] = buf[j + 1];
        buf[10] = next;
    }

    /* After the loop, buf[10] contains a(n). */
    return buf[10];
}
