/*
 * A290891  p-INVERT of the positive integers, where p(S) = 1 - S^3.
 * https://oeis.org/A290891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 21*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=6, a(4)=21,
 *            a(5)=57
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Early values (n < 6) */
    static const seq_t init[6] = {(seq_t)0, (seq_t)0, (seq_t)1,
                                 (seq_t)6, (seq_t)21, (seq_t)57};

    if (n < 6) {
        return init[n];
    }

    /* Buffer holding a(n-6) .. a(n-1) */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)6 * buf[5]            /* 6*a(n-1) */
                   - (seq_t)15 * buf[4]           /* -15*a(n-2) */
                   + (seq_t)21 * buf[3]           /* +21*a(n-3) */
                   - (seq_t)15 * buf[2]           /* -15*a(n-4) */
                   + (seq_t)6 * buf[1]            /* +6*a(n-5) */
                   - buf[0];                     /* -a(n-6) */

        /* Shift buffer left by one position */
        for (int j = 0; j < 5; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[5] = next;
    }

    return buf[5];   /* a(n) */
}
