/*
 * A291382  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 2 S - S^2.
 * https://oeis.org/A291382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4),
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=2, a(1)=7, a(2)=22, a(3)=70, a(4)=222 */

seq_t a(idx_t n) {
    /* Initial terms */
    static const seq_t init[5] = {
        (seq_t)2,    /* a(0) */
        (seq_t)7,    /* a(1) */
        (seq_t)22,   /* a(2) */
        (seq_t)70,   /* a(3) */
        (seq_t)222   /* a(4) */
    };

    if (n < 5) {
        return init[n];
    }

    /* Recurrence: a(n) = 2*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4) */
    seq_t t1 = init[1];  /* a(n-3) */
    seq_t t2 = init[2];  /* a(n-2) */
    seq_t t3 = init[3];  /* a(n-1) */
    seq_t t4 = init[4];  /* a(n) for n=4 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 2 * t4 + 3 * t3 + 2 * t2 + t1;
        /* shift values for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }

    return t4;  /* a(n) */
}
