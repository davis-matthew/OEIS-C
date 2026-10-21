/*
 * A277545  a(n) = n/7^m mod 7, where 7^m is the greatest power of 7 that
 *          divides n.
 * https://oeis.org/A277545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / 7^m mod 7, where 7^m is the greatest power of 7 that
 *            divides n. Equivalent: rightmost nonzero digit of n in base 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No special cases needed; n ≥ 1 due to the sequence offset. */
    idx_t t = n;
    /* Remove all factors of 7. */
    while ((t % 7) == 0)
        t /= 7;
    /* Return the remaining remainder modulo 7. */
    return (seq_t)(t % 7);
}
