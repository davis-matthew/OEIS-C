/*
 * A305739  a(n) = n!*T(n) - 1, where T(n) is the n-th triangular number.
 * https://oeis.org/A305739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)!/2 - 1 (n >= 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the implicit offset */
    if (n == 0)
        return 0;          /* not used in the OEIS sequence, safe fallback */

    /* Compute n! in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute T(n) = n(n+1)/2 in seq_t */
    seq_t tmp = (seq_t)n * ((seq_t)n + 1);
    tmp /= 2;

    /* a(n) = n! * T(n) - 1 */
    seq_t result = fact * tmp;
    result -= 1;

    return result;
}
