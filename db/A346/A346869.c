/*
 * A346869  Sum of all divisors, except the smallest and the largest of every
 *          number, of the first n odd numbers.
 * https://oeis.org/A346869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^n ( σ(2k-1) - (2k-1) - 1 ) for 2k-1 > 1, and 0
 *            for 2k-1 = 1, where σ(x) is the sum of all positive divisors of
 *            x
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t limit = 2 * n - 1;
    /* Allocate array to hold divisor sums, zero-initialised */
    seq_t *sum = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (sum == NULL)
        return (seq_t)0;   /* allocation failure, return 0 */

    /* Compute σ(m) for all m ≤ limit using the classic divisor-sum sieve */
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i)
            sum[j] += val;
    }

    /* Accumulate the desired partial sums for the first n odd numbers */
    seq_t acc = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = 2 * k - 1;
        seq_t contribution;
        if (m == 1)
            contribution = (seq_t)0;
        else
            contribution = sum[m] - m - (seq_t)1;
        acc += contribution;
    }

    free(sum);
    return acc;
}
