/*
 * A277544  a(n) = n/6^m mod 6, where 6^m is the greatest power of 6 that
 *          divides n.
 * https://oeis.org/A277544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/6^m mod 6, where 6^m is the greatest power of 6 that
 *            divides n Equivalent to the right‑most non‑zero digit in the
 *            base‑6 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    /* Strip away all factors of 6 */
    while ((m % 6ULL) == 0)
        m /= 6ULL;
    /* Now m is not divisible by 6; its remainder mod 6 is the desired value */
    return (seq_t)(m % 6ULL);
}
