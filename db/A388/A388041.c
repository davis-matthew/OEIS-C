/*
 * A388041  a(n) = n! * ((n-2) * 2^(n-1) + 1).
 * https://oeis.org/A388041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ((n-2) * 2^(n-1) + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute 2^(n-1) by repeated doubling */
    seq_t pow2 = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow2 *= 2;

    /* term = (n-2) * 2^(n-1) + 1 */
    seq_t term = ((seq_t)(n - 2)) * pow2 + 1;

    return fact * term;
}
