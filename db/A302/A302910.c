/*
 * A302910  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 6-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1/2) * (2*n-1)! / (3 * 2^(n-2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1) = 1; a(n) = a(n-1) * (n-1) * (2n+1) for n >= 2 */

seq_t a(idx_t n)
{
    /* Guard against zero index (not used by OEIS) */
    if (n == 0) return 0;

    seq_t result = 1;            /* a(1) = 1 */
    if (n == 1) return result;

    /* Iterate using the recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Multiply by (i-1) and (2*i+1) */
        result = result * ((seq_t)(i - 1)) * ((seq_t)(2 * i + 1));
    }
    return result;
}
