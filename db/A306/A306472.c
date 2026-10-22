/*
 * A306472  a(n) = 37*27^n.
 * https://oeis.org/A306472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37*27^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)37;

    seq_t result = 1;
    seq_t base   = 27;
    idx_t  exp   = n;

    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;   /* square for next bit */
    }

    result *= (seq_t)37;
    return result;
}
