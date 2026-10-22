/*
 * A305225  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-4) - a(n-5) + 2*a(n-6) for n>9,
 *            with initial values: a(1)=4, a(2)=21, a(3)=24, a(4)=54,
 *            a(5)=108, a(6)=236, a(7)=509, a(8)=1091, a(9)=2363
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[10] = {0,4,21,24,54,108,236,509,1091,2363};
    if (n <= 9) {
        return base[n];
    }
    /* Bootstrap the sliding window for recurrence */
    seq_t a_n1 = base[9]; /* a(n-1)  */
    seq_t a_n2 = base[8]; /* a(n-2)  */
    seq_t a_n3 = base[7]; /* a(n-3)  */
    seq_t a_n4 = base[6]; /* a(n-4)  */
    seq_t a_n5 = base[5]; /* a(n-5)  */
    seq_t a_n6 = base[4]; /* a(n-6)  */
    idx_t i;
    for (i = 10; i <= n; ++i) {
        seq_t next = 2*a_n1 + a_n2 + 2*a_n6;
        next -= 3*a_n4;
        next -= a_n5;
        /* shift window */
        a_n6 = a_n5;
        a_n5 = a_n4;
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = next;
    }
    return a_n1;
}
