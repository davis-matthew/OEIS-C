/*
 * A269735  G.f.: Sum_{k >= 0} x^(2^k)*(1-x^(2^k))/(1+x^(2^k)).
 * https://oeis.org/A269735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=0}^{v2(n)} f(n >> k), where * f(m) = 1 if m == 1 *
 *            2 if m is odd and > 1 * -2 if m is even and > 1 * The exponent
 *            v2(n) is the number of trailing zero bits in n. * This follows
 *            from the generating function * Sum_{k>=0} x^{2^k} (1 -
 *            x^{2^k})/(1 + x^{2^k})
 * Range:     i8 n <= 8765, i16 n <= 8765, i32 n <= 8765, i64 n <= 8765,
 *            i128 n <= 8765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute v2(n) – the largest k such that 2^k divides n. */
    idx_t temp = n;
    idx_t v2 = 0;
    while ((temp & 1) == 0)
    {
        temp >>= 1;
        ++v2;
    }

    seq_t sum = 0;
    idx_t m = n;
    for (idx_t k = 0; k <= v2; ++k)
    {
        if (m == 1)
            sum += 1;
        else
            sum += (m & 1) ? 2 : -2;

        m >>= 1;   /* next higher power of two */
    }
    return sum;
}
