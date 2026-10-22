/*
 * A316631  Expansion of A(x) = x*(1+3*x^2+x^3+3*x^4+x^6)/(1-x^4)^2.
 * https://oeis.org/A316631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = (n%4==0) ? n/4 : (n%2==1) ? n : 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int rem4 = (unsigned int)(n & 3u);
    if (rem4 == 0) {
        return (seq_t)n / 4;
    } else if (rem4 == 2) {
        return 0;
    } else {                 /* rem4 == 1 or 3 → n is odd */
        return (seq_t)n;
    }
}
