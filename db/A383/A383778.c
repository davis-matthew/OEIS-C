/*
 * A383778  a(n) = n*(n^2 - 3*n + 10)*2^(n-4).
 * https://oeis.org/A383778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 3*n + 10)*2^(n-4)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type once */
    seq_t tn = (seq_t)n;

    /* Compute n^2 */
    seq_t n_sq = tn * tn;

    /* n^2 - 3*n + 10 */
    seq_t part = n_sq - 3 * tn + (seq_t)10;

    /* Base = n * (n^2 - 3*n + 10) */
    seq_t base = tn * part;

    if (n >= 4)
    {
        /* Multiply by 2^(n-4) by repeated left shifts */
        /* (N-4) is at most 9996 for N <= 10000, well within a loop */
        for (idx_t i = 0; i < n - 4; ++i)
            base <<= 1;
    }
    else
    {
        /* For n < 4, divide by 2^(4-n) which is a power of two at most 8 */
        base >>= 4 - n;   /* safe: 4-n <= 3 */
    }

    return base;
}
