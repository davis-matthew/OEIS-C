/*
 * A271391  Expansion of (1 + x + 2*x^2 + 6*x^3 + x^4 + x^5)/(1 - x^2)^3.
 * https://oeis.org/A271391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0)?2*(n/2)*(n/2+1)+1:n*n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {
        return (seq_t)n * (seq_t)n;
    } else {
        seq_t k = (seq_t)(n >> 1);
        return 2 * k * (k + 1) + 1;
    }
}
