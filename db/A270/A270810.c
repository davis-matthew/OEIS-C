/*
 * A270810  Expansion of (x - x^2 + 2*x^3 + 2*x^4)/(1 - 3*x + 2*x^2).
 * https://oeis.org/A270810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(2)=2; a(3)=6; a(4)=16; a(n)=2*a(n-1)+4 for
 *            n>=5. (Equivalent to a(n)=5*2^(n-2)-4 for n>2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[5] = {0, 1, 2, 6, 16};
    if (n <= 4u)
        return base[n];
    seq_t val = 16;               /* a(4) */
    for (idx_t i = 5u; i <= n; ++i)
        val = val * 2 + 4;        /* recurrence a(n)=2*a(n-1)+4 */
    return val;
}
