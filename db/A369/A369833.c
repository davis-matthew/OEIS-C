/*
 * A369833  a(n) is the number of distinct values of the permanent of an n X n
 *          symmetric Toeplitz matrix using the first n prime numbers.
 * https://oeis.org/A369833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(0)=1, a(1)=1, a(2)=1; for n≥3 the value is n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as defined for OEIS sequence A369833 */
seq_t a(idx_t n)
{
    /* The first three terms are fixed to 1 */
    if (n <= 2)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* Compute the factorial of n */
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;   /* multiply by i, cast to seq_t   */
    return result;
}
