/*
 * A298784  Expansion of (1 + x^2)*(1 + 3*x + x^2) / ((1 - x)*(1 - x^3)).
 * https://oeis.org/A298784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; * a(3*k) = 10*k * a(3*k+1) = 10*k + 4 * a(3*k+2) = 10*k
 *            + 6 * (derived from the generating function
 *            (1+x²)(1+3x+x²)/((1-x)(1-x³)))
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    idx_t k = n / 3u;
    idx_t r = n % 3u;
    seq_t result = (seq_t)10 * (seq_t)k;
    if (r == 1u)
        result += (seq_t)4;
    else if (r == 2u)
        result += (seq_t)6;
    return result;
}
