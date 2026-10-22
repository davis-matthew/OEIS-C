/*
 * A359239  Number of divisors of 3*n-2 of form 3*k+2.
 * https://oeis.org/A359239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 3*n‑2 that are congruent to 2
 *            modulo 3
 * Range:     u8 n <= 85, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = 3*n - 2 */
    seq_t m = (seq_t)n * ((seq_t)3) - (seq_t)2;
    seq_t count = 0;
    /* iterate over all divisor candidates up to sqrt(m) */
    for (seq_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            seq_t d2 = m / d;          /* paired divisor */
            if ((d % 3) == 2) ++count;  /* divisor d satisfies mod 3 == 2 */
            if (d2 != d && (d2 % 3) == 2) ++count; /* paired divisor */
        }
    }
    return count;
}
