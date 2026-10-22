/*
 * A334513  Eventual period of a single cell in rule 25 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) Initial values: a(1)=2, a(2)=2, a(3)=3,
 *            a(4)=2, a(5)=15, a(6)=15, a(7)=21, a(8)=16, a(9)=9, a(10)=20,
 *            a(11)=22 For n>11 the recurrence holds
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[11] = {
    (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)2, (seq_t)15,
    (seq_t)15, (seq_t)21, (seq_t)16, (seq_t)9, (seq_t)20, (seq_t)22
};

seq_t a(idx_t n)
{
    /* Handle defined range */
    if (n <= 11)
        return base[n - 1];

    /* Apply recurrence for n > 11 */
    seq_t a_nm2 = base[9];   /* a(10) */
    seq_t a_nm1 = base[10];  /* a(11) */
    seq_t a_cur  = 0;

    for (idx_t i = 12; i <= n; ++i) {
        a_cur = (2 * a_nm1) - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}
