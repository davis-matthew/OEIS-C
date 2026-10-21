/*
 * A268444  a(n) = Product_{i=0..k}(n_i+1) where n = Sum_{i=0..k}n_i*4^i is
 *          the base-4 representation of n.
 * https://oeis.org/A268444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{i=0..k}(n_i+1) where n = Σ n_i·4^i (base‑4
 *            digits)
 * Range:     u8 n <= 254, u16 n <= 8854, u32 n <= 8854, u64 n <= 8854,
 *            u128 n <= 8854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    while (n > 0) {
        idx_t digit = n & 3U;            /* current base‑4 digit */
        result *= (seq_t)(digit + 1);    /* multiply by (digit+1) */
        n >>= 2;                         /* divide by 4 */
    }
    return result;
}
