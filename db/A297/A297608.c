/*
 * A297608  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 4*a(n-3) + 12*a(n-4) - 12*a(n-6)
 *            Empirical recurrence with initial terms: a(1)=4, a(2)=11,
 *            a(3)=35, a(4)=96, a(5)=281, a(6)=829
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for indices 1..6. */
    static const seq_t base[7] = {0, (seq_t)4, (seq_t)11, (seq_t)35,
                                  (seq_t)96, (seq_t)281, (seq_t)829};

    if (n <= 6)
        return base[n];

    seq_t a0 = base[1]; /* a(n-6) when k = 7 */
    seq_t a1 = base[2]; /* a(n-5) */
    seq_t a2 = base[3]; /* a(n-4) */
    seq_t a3 = base[4]; /* a(n-3) */
    seq_t a4 = base[5]; /* a(n-2) */
    seq_t a5 = base[6]; /* a(n-1) */

    for (idx_t k = 7; k <= n; ++k) {
        seq_t next = a5 + (seq_t)3 * a4 + (seq_t)4 * a3 +
                     (seq_t)12 * a2 - (seq_t)12 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
