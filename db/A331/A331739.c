/*
 * A331739  a(n) is n minus its largest odd divisor.
 * https://oeis.org/A331739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - A000265(n), where A000265(n) is the largest odd
 *            divisor of n
 * Range:     u8 n <= 287, u16 n <= 9976, u32 n <= 9976, u64 n <= 9976,
 *            u128 n <= 9976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find v2(n), the exponent of the highest power of 2 dividing n */
    idx_t tmp = n;
    idx_t v = 0;
    while ((tmp & 1) == 0)
    {
        tmp >>= 1;
        ++v;
    }

    /* odd part = n / 2^v  (or n >> v) */
    seq_t odd = ((seq_t)n) >> v;   /* shift is valid on seq_t */

    /* a(n) = n - odd */
    return ((seq_t)n) - odd;
}
