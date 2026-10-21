/*
 * A282554  Number of nX2 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence a(n) = 6*a(n-1) - 7*a(n-2) - 2*a(n-3) - 29*a(n-4) +
 *            44*a(n-5) + 12*a(n-6) + 32*a(n-7) - 64*a(n-8) Initial values:
 *            a(1)=0, a(2)=0, a(3)=10, a(4)=36, a(5)=154, a(6)=652, a(7)=2472,
 *            a(8)=9356
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values up to n = 8.  Index 0 is unused. */
    static const seq_t init[9] = {
        0,      /* a(0) – not used */
        0,      /* a(1) */
        0,      /* a(2) */
        10,     /* a(3) */
        36,     /* a(4) */
        154,    /* a(5) */
        652,    /* a(6) */
        2472,   /* a(7) */
        9356    /* a(8) */
    };

    if (n <= 8)
        return init[n];

    /* Buffer for the last 8 sequence values:
       buf[0] = a(k-8), buf[1] = a(k-7), … , buf[7] = a(k-1)  */
    seq_t buf[8] = {
        init[1], init[2], init[3], init[4],
        init[5], init[6], init[7], init[8]
    };

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute the new value using only 64‑bit integer arithmetic
           on the (unsigned) seq_t type.  The expression is rewritten
           as positive_sum - negative_sum to avoid unsigned wrap‑around. */
        seq_t pos = 6*buf[7] + 44*buf[3] + 12*buf[2] + 32*buf[1];
        seq_t neg = 7*buf[6] + 2*buf[5] + 29*buf[4] + 64*buf[0];
        seq_t new_val = pos - neg;

        /* Shift the buffer left to discard a(k-8) and make room for the
           new value.  This is the simplest and fastest way for a small
           fixed‑size array. */
        for (int i = 0; i < 7; ++i)
            buf[i] = buf[i + 1];
        buf[7] = new_val;
    }

    /* buf[7] now holds a(n). */
    return buf[7];
}
