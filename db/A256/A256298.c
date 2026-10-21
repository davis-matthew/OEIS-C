/*
 * A256298  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0
 *          to the digits of n written in base 8, then convert back to base
 *          10.
 * https://oeis.org/A256298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret(n in base 8, replace each digit d by (d+1) mod
 *            8, then read the modified digits as a base 8 number
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Store the octal digits of n (least‑significant first) */
    uint8_t dig[64];
    idx_t    t   = n;
    idx_t    len = 0;
    do {
        dig[len++] = t & 7;   /* t % 8 */
        t >>= 3;              /* t / 8 */
    } while (t);

    /* Build the result by processing most‑significant digit first */
    seq_t res = 0;
    for (idx_t i = len; i-- > 0; ) {
        seq_t d  = dig[i];
        seq_t nd = (d + 1) & 7;   /* (d+1) mod 8 */
        res = res * 8 + nd;
    }
    return res;
}
