/*
 * A345030  a(n) = Sum_{k=1..n} n^(floor(n/k) - 1).
 * https://oeis.org/A345030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} n^(floor(n/k)-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is undefined for the sequence, but we return 0
       to avoid undefined behaviour in low-level calls. */
    if (n == 0) return 0;

    seq_t result = 0;        /* Accumulator for the sum   */
    seq_t power  = 1;        /* Holds n^(m-1) for current m */
    seq_t base   = (seq_t)n; /* The base n, cast to seq_t   */

    /* We iterate over m = 1..n, where m = floor(n/k).  For each m
       the exponent of n in the term is (m-1), and the multiplicity
       of this exponent is: floor(n/m) - floor(n/(m+1)).          */
    for (idx_t m = 1; m <= n; ++m)
    {
        /* Number of k such that floor(n/k) == m. */
        idx_t count = (n / m) - (n / (m + 1));

        if (count != 0)
        {
            /* Add count * n^(m-1) to the total. */
            result += power * (seq_t)count;
        }

        /* Prepare power for the next iteration (n^m). */
        power *= base;
    }

    return result;
}
