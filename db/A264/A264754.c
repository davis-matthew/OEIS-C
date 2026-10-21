/*
 * A264754  Expansion of (1 + 2*x - 2*x^3 + x^4)/((1 - x)^3*(1 + x)^2).
 * https://oeis.org/A264754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n/2⌋² + 3·⌊n/2⌋ + (n is even ? 1 : 3) derived from a(n)
 *            = (2*n*(n + (-1)^n + 5) - 3*(-1)^n + 11)/8
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    seq_t k = (seq_t)(n >> 1);
    seq_t k2 = k * k;
    seq_t val = k2 + 3 * k;
    /* add 1 for even n, 3 for odd n */
    if ((n & 1) == 0)
        val += 1;
    else
        val += 3;
    return val;
}
