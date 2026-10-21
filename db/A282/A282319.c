/*
 * A282319  a(n) = (2097203 mod n)^2 + (2097203 mod n) + 41.
 * https://oeis.org/A282319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2097203 mod n)^2 + (2097203 mod n) + 41 (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2097203 is the fixed modulus constant */
    const seq_t MOD = (seq_t)2097203;
    /* Compute the remainder r = MOD mod n */
    const seq_t r = MOD % (seq_t)n;
    /* Compute r^2 + r + 41 */
    return r * r + r + (seq_t)41;
}
