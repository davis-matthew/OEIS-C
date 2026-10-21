/*
 * A291390  a(n) = (1/5)*A291389(n).
 * https://oeis.org/A291390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 15*a(n-2) - 50*a(n-3) - 25*a(n-4) for n≥5,
 *            a(0)=2, a(1)=17, a(2)=130, a(3)=940, a(4)=6550
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial 5 terms */
    if (n < 5) {
        static const seq_t init[5] = {(seq_t)2, (seq_t)17, (seq_t)130,
                                      (seq_t)940, (seq_t)6550};
        return init[n];
    }
    /* Rolling buffer for previous 5 terms */
    seq_t a0 = (seq_t)2;   /* a(n-4) */
    seq_t a1 = (seq_t)17;  /* a(n-3) */
    seq_t a2 = (seq_t)130; /* a(n-2) */
    seq_t a3 = (seq_t)940; /* a(n-1) */
    seq_t a4 = (seq_t)6550;/* a(n)  for n>=4 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)10 * a4;
        next -= (seq_t)15 * a3;
        next -= (seq_t)50 * a2;
        next -= (seq_t)25 * a1;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
