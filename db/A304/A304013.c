/*
 * A304013  Number of nX2 0..1 arrays with every element unequal to 1, 2, 3 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +5*a(n-2) -2*a(n-3) -12*a(n-4) -8*a(n-5) for
 *            n>6; Initial values: a(1)=1, a(2)=7, a(3)=16, a(4)=45, a(5)=120,
 *            a(6)=333
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)45;
    if (n == 5) return (seq_t)120;
    if (n == 6) return (seq_t)333;
    seq_t a2 = (seq_t)7;
    seq_t a3 = (seq_t)16;
    seq_t a4 = (seq_t)45;
    seq_t a5 = (seq_t)120;
    seq_t a6 = (seq_t)333;
    for (idx_t i = (idx_t)7; i <= n; ++i) {
        seq_t next = (seq_t)2 * a6 + (seq_t)5 * a5
                   - (seq_t)2 * a4 - (seq_t)12 * a3
                   - (seq_t)8 * a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}
