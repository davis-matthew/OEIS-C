/*
 * A351912  Period of binary representation of 1/n, or 0 if 1/n terminates.
 * https://oeis.org/A351912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n is a power of two; otherwise a(n) is the
 *            multiplicative order of 2 modulo the odd part of n, i.e. the
 *            smallest t>0 such that 2^t ≡ 1 (mod n/2^v₂(n))
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remove all factors of 2 */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    /* If n is a power of two (or n==1), the binary expansion terminates */
    if (m == 1)
        return 0;

    /* Find the multiplicative order of 2 modulo m (m is odd) */
    seq_t cur = (seq_t)2 % (seq_t)m;
    seq_t period = 1;
    while (cur != 1) {
        /* Use 128-bit multiplication to avoid overflow for 64-bit values */
        cur = ((unsigned __int128)cur * 2) % (seq_t)m;
        period++;
    }
    return period;
}
