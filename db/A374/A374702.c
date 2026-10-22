/*
 * A374702  Number of integer compositions of n whose leaders of maximal
 *          weakly decreasing runs sum to 3. Column k = 3 of A374748.
 * https://oeis.org/A374702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6) (n ≥ 6)
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=3, a(5)=6
 * Range:     u8 n <= 28, u16 n <= 444, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base cases for n < 6 */
    const seq_t base[6] = {0, 0, 0, 2, 3, 6};
    if (n < 6) {
        return base[n];
    }

    /* Allocate an array of size n+1 to hold the sequence values */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) { /* allocation failed, return 0 as a safe fallback */
        return 0;
    }

    /* Initialize first six terms */
    seq[0] = 0;
    seq[1] = 0;
    seq[2] = 0;
    seq[3] = 2;
    seq[4] = 3;
    seq[5] = 6;

    /* Compute for i = 6 .. n using the recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        seq[i] = seq[i - 1] + seq[i - 2] - seq[i - 4] - seq[i - 5] + seq[i - 6];
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
