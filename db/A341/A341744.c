/*
 * A341744  a(0)=1, a(1)=2; for n > 1, a(n) = a(n - a(n-2)) + n.
 * https://oeis.org/A341744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 if n % 4 == 0 or 1, else n + 2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned 64-bit integer.  The result fits in seq_t. */
    idx_t mod4 = n & 3;
    if (mod4 < 2)
        return (seq_t)n + 1;   /* n + 1 for n ≡ 0,1 (mod 4) */
    else
        return (seq_t)n + 2;   /* n + 2 for n ≡ 2,3 (mod 4) */
}
