/*
 * A254651  Characteristic function of A254614, numbers that are either odd or
 *          evil (or both).
 * https://oeis.org/A254651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is odd or the number of 1‑bits in n is even;
 *            otherwise 0
 * Range:     u8 n <= 9448, u16 n <= 9448, u32 n <= 9448, u64 n <= 9448,
 *            u128 n <= 9448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;             /* work on a 64‑bit copy of n */
    /* compute parity of the popcount: 0 = even, 1 = odd */
    uint8_t popparity = 0;
    while (x) {
        popparity ^= (x & 1);
        x >>= 1;
    }
    /* (n & 1) gives 1 for odd numbers; popparity==0 for even popcount */
    if ((n & 1) || (popparity == 0))
        return (seq_t)1;
    else
        return (seq_t)0;
}
