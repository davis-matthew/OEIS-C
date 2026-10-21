/*
 * A246591  Smallest number that can be obtained by swapping 2 bits in the
 *          binary expansion of n.
 * https://oeis.org/A246591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = result of swapping the most significant 1 with the least
 *            significant 0 in the binary representation of n (if no such 0
 *            exists, a(n)=n)
 * Range:     u8 n <= 510, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle zero explicitly */
    if (n == 0)
        return (seq_t)0;

    /* Find position of most significant 1 */
    idx_t msb = 0;                /* msb position (0‑based) */
    idx_t tmp = n;
    while (tmp > 1) {            /* shift right until only the top bit remains */
        tmp >>= 1;
        msb++;
    }

    /* Find the least significant 0 bit at or below 'msb' */
    idx_t low = 0;
    while (low <= msb) {
        if (((n >> low) & (idx_t)1) == 0)
            break;                /* found the last 0 */
        low++;
    }

    /* If no zero found below msb, return n unchanged */
    if (low > msb)
        return (seq_t)n;

    /* Swap the bits by toggling both positions */
    seq_t mask = ((seq_t)1 << msb) | ((seq_t)1 << low);
    return ((seq_t)n) ^ mask;
}
