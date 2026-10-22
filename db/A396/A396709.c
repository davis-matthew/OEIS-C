/*
 * A396709  Partial sums of A396280.
 * https://oeis.org/A396709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26 + 4*((n-1)*(n-2))^2 for n >= 3; a(1) = 2, a(2) = 10
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 182, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)10;

    /* compute ((n-1)*(n-2))^2 safely in seq_t */
    seq_t p = (seq_t)(n - 1) * (seq_t)(n - 2);
    p = p * p;          /* square */
    p = p * (seq_t)4;   /* multiply by 4 */
    p += (seq_t)26;     /* add constant 26 */

    return p;
}
