/*
 * A326471  Sum of the third largest parts of the partitions of n into 9
 *          parts.
 * https://oeis.org/A326471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n-9, 9) + Σ_{t=3}^{9} Σ_{k=1}^{⌊(n-9)/t⌋} p(n-9 - k·t,
 *            9) where p(m,9) is the number of partitions of m into parts of
 *            size at most 9. This follows from writing a partition of n into
 *            nine positive parts as 1 + d1, 1 + d2, …, 1 + d9 with d1 ≥ d2 ≥
 *            … ≥ d9 ≥ 0. Then Σ i·di = n-9 and the thir...
 * Range:     u8 n <= 21, u16 n <= 47, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* partitions with less than nine parts are impossible */
    if (n < 9)
        return (seq_t)0;

    /* S is the sum after removing the mandatory '1' from each of nine parts */
    idx_t S = n - 9;

    /* Allocate array for partitions into parts ≤ 9 up to S */
    seq_t *f = (seq_t *)calloc((size_t)(S + 1), sizeof(seq_t));
    if (!f)
        return (seq_t)0;            /* allocation failed – unlikely but safe */

    f[0] = (seq_t)1;                /* one way to partition 0 */

    /* Compute p(m,9) for m = 0…S via the standard unbounded knapsack DP */
    for (idx_t part = 1; part <= 9; ++part) {
        for (idx_t s = part; s <= S; ++s) {
            f[s] += f[s - part];
        }
    }

    /* Base contribution: the number of partitions of n into 9 parts */
    seq_t result = f[S];

    /* Add contributions from the third-largest part using the derived formula */
    for (idx_t t = 3; t <= 9; ++t) {
        for (idx_t prod = t; prod <= S; prod += t) {
            result += f[S - prod];
        }
    }

    free(f);
    return result;
}
