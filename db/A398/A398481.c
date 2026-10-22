/*
 * A398481  a(n) = Sum_{k=0..n} n^n * exp(1/n) * Gamma(n - k + 1, 1/n) for n
 *          >= 1, a(0) = 1. Row sums of A398480.
 * https://oeis.org/A398481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} n^{n-j} * (Sum_{r=j..n} r!) / j!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is O(n) and uses only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;           /* fact = n! */

    /* Suffix sum of factorials: Suf[j] = sum_{r=j..n} r! at start j = n */
    seq_t suffix = fact;            /* for j = n */
    seq_t power  = 1;               /* power = n^{n-j} ; starts with n^{0} = 1 at j = n */
    seq_t result = 0;

    idx_t j = n;
    while (1)
    {
        /* term = n^{n-j} * (sum_{r=j..n} r!) / j! */
        seq_t term = (power * suffix) / fact;
        result += term;

        if (j == 0)
            break;

        /* Prepare values for j-1 */
        power *= (seq_t)n;    /* increase exponent: n^{n-(j-1)} = n^{n-j} * n */
        fact /= (seq_t)j;     /* fact now = (j-1)!  */
        suffix += fact;       /* suffix now = sum_{r=j-1..n} r! */

        --j;
    }

    return result;
}
