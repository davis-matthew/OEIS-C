/*
 * A382709  Numerator of (2^n - 1)*n! / 2^(n+1).
 * https://oeis.org/A382709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 0; For n ≥ 1, a(n) = (2^n – 1) · (n! / 2^v₂(n!)), i.e.
 *            (2^n – 1) multiplied by the odd part of n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t odd_part = 1;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t m = k;
        /* strip all factors of 2 from k */
        while ((m & 1) == 0)
            m >>= 1;
        odd_part *= (seq_t)m;   /* multiply only the odd remainder */
    }

    /* compute 2^n */
    seq_t pow2n = 1;
    pow2n <<= n;                /* pow2n = 2^n */

    /* numerator is  (2^n – 1) * odd_part   */
    return (pow2n - 1) * odd_part;
}
