/*
 * A309952  XOR contraction of binary representation of n.
 * https://oeis.org/A309952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(0) = 0 a(4n) = 2*a(n) a(4n+1) = 2*a(n) + 1 a(4n+2) = 2*a(n) +
 *            1 a(4n+3) = 2*a(n) This recurrence implements the
 *            XOR‑contraction of the binary representation of n
 * Range:     u8 n <= 8794, u16 n <= 8794, u32 n <= 8794, u64 n <= 8794,
 *            u128 n <= 8794, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;                 /* base case */

    /* Recursively compute a(n/4) */
    seq_t res = a(n / 4U);

    /* Determine the contribution of the last two binary digits */
    idx_t r = n & 3U;              /* r == n mod 4 */

    if (r == 0U || r == 3U)
        return res << 1;           /* XOR 0 XOR 0 = 0, 1 XOR 1 = 0 */

    /* r == 1 or 2: XOR 0 XOR 1 or 1 XOR 0 = 1 */
    return (res << 1) | (seq_t)1;
}
