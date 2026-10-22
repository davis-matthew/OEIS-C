/*
 * A339418  Number of compositions (ordered partitions) of n into an even
 *          number of squares.
 * https://oeis.org/A339418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into an even number of
 *            squares Computed by dynamic programming: even[i] = counts for
 *            compositions of i with even number of squares, odd[i] for odd
 *            number of squares
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for even and odd length counts. */
    seq_t *even;
    seq_t *odd;
#ifdef __cplusplus
    even = new seq_t[(size_t)(n + 1)];
    odd  = new seq_t[(size_t)(n + 1)];
    for (idx_t i = 0; i <= n; ++i) {
        even[i] = (seq_t)0;
        odd[i]  = (seq_t)0;
    }
#else
    even = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    odd  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif

    /* Base case: empty composition of sum 0 has even length 0. */
    even[0] = (seq_t)1;

    /* Dynamic programming: for each sum i, sum over all square parts. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum_even = (seq_t)0;
        seq_t sum_odd  = (seq_t)0;

        for (idx_t s = 1; s * s <= i; ++s) {
            idx_t sq = s * s;          /* s² ≤ i, fits in idx_t */
            idx_t rem = i - sq;        /* remaining sum, fits in idx_t */
            sum_even += odd[rem];
            sum_odd  += even[rem];
        }

        even[i] = sum_even;
        odd[i]  = sum_odd;
    }

    seq_t result = even[n];

    /* Clean up memory. */
#ifdef __cplusplus
    delete[] even;
    delete[] odd;
#else
    free(even);
    free(odd);
#endif

    return result;
}
