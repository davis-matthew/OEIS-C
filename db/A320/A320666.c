/*
 * A320666  a(n) is the maximum number of liberties a single group can have on
 *          an otherwise empty n X n Go board.
 * https://oeis.org/A320666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0, a(2)=2, a(3)=6; for n>3: if n mod 3 =0 a(n)=n*(2*n-1)/3;
 *            if n mod 3 =1 a(n)=((2*n-1)^2+5)/6; if n mod 3 =2
 *            a(n)=((2*n-1)^2+3)/6
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    seq_t ns = (seq_t)n;
    idx_t r = n % 3;

    if (r == 0) {
        seq_t t = (seq_t)2 * ns - (seq_t)1;
        return ns * t / (seq_t)3;
    }
    if (r == 1) {
        seq_t t = (seq_t)2 * ns - (seq_t)1;
        seq_t val = t * t + (seq_t)5;
        return val / (seq_t)6;
    }
    /* r == 2 */
    seq_t t = (seq_t)2 * ns - (seq_t)1;
    seq_t val = t * t + (seq_t)3;
    return val / (seq_t)6;
}
