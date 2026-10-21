/*
 * A278443  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 43", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3) + 100*a(n-4) -
 *            100*a(n-5) - 10000*a(n-6) + 10000*a(n-7), with initial values
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1101, a(4)=101, a(5)=110101,
 *            a(6)=101
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms as given */
    static const seq_t init[7] = {
        (seq_t)1,        /* a(0) */
        (seq_t)1,        /* a(1) */
        (seq_t)1,        /* a(2) */
        (seq_t)1101,     /* a(3) */
        (seq_t)101,      /* a(4) */
        (seq_t)110101,   /* a(5) */
        (seq_t)101       /* a(6) */
    };

    if (n <= 6)
        return init[n];

    /* buffer holding the last 7 values a[i-6]..a[i] during iteration */
    seq_t buf[7];
    for (int k = 0; k < 7; ++k)
        buf[k] = init[k];

    for (idx_t i = 7; i <= n; ++i) {
        /* compute a(i) from a(i-1)..a(i-7) */
        seq_t next = buf[6];                         /* buf[6] == a(i-1) */
        next += (seq_t)100 * buf[5];                 /* +100*a(i-2) */
        next -= (seq_t)100 * buf[4];                 /* -100*a(i-3) */
        next += (seq_t)100 * buf[3];                 /* +100*a(i-4) */
        next -= (seq_t)100 * buf[2];                 /* -100*a(i-5) */
        next -= (seq_t)10000 * buf[1];               /* -10000*a(i-6) */
        next += (seq_t)10000 * buf[0];               /* +10000*a(i-7) */

        /* rotate buffer: shift left by one position */
        for (int k = 0; k < 6; ++k)
            buf[k] = buf[k + 1];
        buf[6] = next;   /* buf[6] now holds a(i) */
    }

    return buf[6];
}
