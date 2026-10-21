/*
 * A272966  Number of n X n 0..1 arrays with exactly n+n-2 having value 1 and
 *          no three 1's forming an isosceles triangle.
 * https://oeis.org/A272966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; // special cases a(2)=6; a(3)=12; for n>=4: a(n)=8 when
 *            n is even, a(n)=4 when n is odd (i.e. a(n)=4+4*(n%2==0) for
 *            n>=4)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    /* For n >= 4 */
    if (n & 1) return (seq_t)4; /* odd */
    else return (seq_t)8;        /* even */
}
