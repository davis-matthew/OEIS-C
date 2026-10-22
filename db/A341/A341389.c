/*
 * A341389  Characteristic function of A158705, nonnegative integers with an
 *          odd number of even powers of 2 in their base-2 representation.
 * https://oeis.org/A341389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has an odd number of 1‑bits in even positions of
 *            its binary representation, 0 otherwise. This is equivalent to
 *            the recurrence a(2n)=a(n/2), a(2n+1)=1−a(n/4) with a(0)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count bits at even positions (0,2,4,...) and return parity. */
    idx_t temp = n;
    int parity = 0;           /* will become 0 or 1 */
    while (temp) {
        if (temp & 1U)        /* check current (even) bit */
            parity ^= 1;
        temp >>= 2;           /* skip the odd bit */
    }
    return (seq_t)parity;
}
