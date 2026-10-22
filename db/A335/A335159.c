/*
 * A335159  a(n) = nim-product of 2^2 and 2^n.
 * https://oeis.org/A335159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-4) for n > 3 (with a(0)=4, a(1)=8, a(2)=6,
 *            a(3)=11)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = { (seq_t)4, (seq_t)8, (seq_t)6, (seq_t)11 };
    seq_t result = base[n & 3];
    idx_t k = n >> 2;               /* number of times to multiply by 16 */
    while (k--) result *= (seq_t)16;
    return result;
}
