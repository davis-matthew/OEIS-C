/*
 * A318944  Number of Dyck paths with n nodes and altitude 4.
 * https://oeis.org/A318944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 31*a(n-2) + 50*a(n-3) - 36*a(n-4) + 8*a(n-5)
 *            (n>9) Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 *            a(5)=8, a(6)=32, a(7)=114, a(8)=376, a(9)=1177
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)8, (seq_t)32, (seq_t)114,
        (seq_t)376, (seq_t)1177
    };

    if (n < 10)
        return init[n];

    /* five most recent terms for recurrence */
    seq_t a0 = init[5];  /* a(n-5) for n = 10 */
    seq_t a1 = init[6];
    seq_t a2 = init[7];
    seq_t a3 = init[8];
    seq_t a4 = init[9];  /* a(n-1) */

    seq_t ai;
    for (idx_t i = 10; i <= n; ++i) {
        ai = (seq_t)9 * a4
           - (seq_t)31 * a3
           + (seq_t)50 * a2
           - (seq_t)36 * a1
           + (seq_t)8 * a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
