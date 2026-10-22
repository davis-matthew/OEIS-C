/*
 * A329505  Expansion of (1 + x)*(1 + 2*x - x^2) / (1 - x).
 * https://oeis.org/A329505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=5, and a(n)=4 for all n>=3
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n is unsigned, simple equality checks are safe */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)5;
    /* For all n >= 3 the value is 4 */
    return (seq_t)4;
}
