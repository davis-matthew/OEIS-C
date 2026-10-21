/*
 * A245999  Number of length 4+2 0..n arrays with no pair in any consecutive
 *          three terms totalling exactly n.
 * https://oeis.org/A245999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) -11*a(n-3) +6*a(n-4) +14*a(n-5)
 *            -14*a(n-6) -6*a(n-7) +11*a(n-8) -a(n-9) -3*a(n-10) +a(n-11)
 *            Initial values (offset 1): a(1)=2, a(2)=26, a(3)=396, a(4)=2196,
 *            a(5)=9582, a(6)=29790, a(7)=80504, a(8)=185192, a(9)=391290,
 *            a(10)=753570, a(11)=1371972 The recurrence holds for n...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Pre‑computed first 11 terms (offset 1) */
    static const seq_t init[11] = {
        (seq_t)2,
        (seq_t)26,
        (seq_t)396,
        (seq_t)2196,
        (seq_t)9582,
        (seq_t)29790,
        (seq_t)80504,
        (seq_t)185192,
        (seq_t)391290,
        (seq_t)753570,
        (seq_t)1371972
    };

    if (n <= 11) /* offset 1 */
        return init[n - 1];

    /* Rolling array for the last 11 values:
       last[0] = a(k-1), last[1] = a(k-2), ..., last[10] = a(k-11)   */
    seq_t last[11];
    last[0] = init[10]; /* a(11) */
    last[1] = init[9];  /* a(10) */
    last[2] = init[8];  /* a(9)  */
    last[3] = init[7];  /* a(8)  */
    last[4] = init[6];  /* a(7)  */
    last[5] = init[5];  /* a(6)  */
    last[6] = init[4];  /* a(5)  */
    last[7] = init[3];  /* a(4)  */
    last[8] = init[2];  /* a(3)  */
    last[9] = init[1];  /* a(2)  */
    last[10] = init[0]; /* a(1)  */

    for (idx_t k = 12; k <= n; ++k) {
        seq_t next = (seq_t)3 * last[0] + last[1]
                    - (seq_t)11 * last[2] + (seq_t)6 * last[3]
                    + (seq_t)14 * last[4] - (seq_t)14 * last[5]
                    - (seq_t)6 * last[6] + (seq_t)11 * last[7]
                    - last[8] - (seq_t)3 * last[9] + last[10];

        /* shift the window: newest value becomes last[0] */
        for (int i = 10; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = next;
    }
    return last[0];
}
