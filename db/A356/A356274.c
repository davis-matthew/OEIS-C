/*
 * A356274  a(n) is the number whose base-(n+1) expansion equals the binary
 *          expansion of n.
 * https://oeis.org/A356274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the binary expansion of n as digits in base
 *            (n+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for the OEIS offset, but safe */
    seq_t base   = (seq_t)n + 1;   /* (n+1) as base */
    seq_t result = 0;
    seq_t basepow = 1;             /* current power of (n+1) */
    idx_t temp = n;

    while (temp) {
        if (temp & 1) result += basepow;   /* add the current power if the bit is 1 */
        temp >>= 1;
        basepow = basepow * base;          /* prepare next power */
    }
    return result;
}
