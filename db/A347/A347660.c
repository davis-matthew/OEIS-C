/*
 * A347660  Number of partitions of n into at most 5 powers of 2.
 * https://oeis.org/A347660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..5} number of partitions of n into exactly k
 *            powers of 2 (unordered). This implementation uses dynamic
 *            programming over the allowed part sizes and limits the number of
 *            parts to 5
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Allocate a 2‑D array [k][sum] with k = 0..5 and sum = 0..n. */
    size_t rows = 6;
    size_t cols = (size_t)(n + 1);
    seq_t *parts = (seq_t *)malloc(rows * cols * sizeof(seq_t));
    if (!parts) return (seq_t)0;  /* allocation failure – return 0 */

    /* Helper macro to index the array */
    #define IDX(k, s) ((size_t)(k) * cols + (size_t)(s))

    /* Initialize */
    for (size_t k = 0; k < rows; ++k)
        for (size_t s = 0; s < cols; ++s)
            parts[IDX(k, s)] = 0;
    parts[IDX(0, 0)] = 1;   /* one way to make 0 using 0 parts */

    /* Iterate over all powers of two not exceeding n */
    for (idx_t p = 1; p <= n; p <<= 1)
    {
        /* For each allowable part count k (1..5) */
        for (size_t k = 1; k <= 5; ++k)
        {
            /* Update ways to form sums using an additional part of size p */
            for (idx_t sum = p; sum <= n; ++sum)
            {
                parts[IDX(k, sum)] += parts[IDX(k - 1, sum - p)];
            }
        }
    }

    /* Sum contributions from all part counts 0..5 */
    seq_t result = 0;
    for (size_t k = 0; k <= 5; ++k)
        result += parts[IDX(k, n)];

    free(parts);
    return result;
}
