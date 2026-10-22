/*
 * A374043  a(n) = 1 if n is a non-multiple of 3 whose 2-adic valuation is a
 *          multiple of 3, otherwise 0.
 * https://oeis.org/A374043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is not divisible by 3 and its 2-adic valuation is
 *            a multiple of 3, otherwise 0
 * Range:     u8 n <= 9896, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is a multiple of 3, result is 0 immediately. */
    if (n % 3 == 0)
        return (seq_t)0;

    /* Compute 2-adic valuation v2(n). */
    idx_t v2 = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* Return 1 iff v2 is a multiple of 3. */
    return (v2 % 3 == 0) ? (seq_t)1 : (seq_t)0;
}
