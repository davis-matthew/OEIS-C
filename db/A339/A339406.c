/*
 * A339406  Number of partitions of n into an even number of parts that are
 *          not multiples of 4.
 * https://oeis.org/A339406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of parts
 *            that are not multiples of 4. The value is returned as a seq_t
 *            and the computation uses a standard DP over the allowed part
 *            sizes
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */

seq_t a(idx_t n)
{
    /* Allocate arrays to hold counts of partitions with even or odd
       number of parts for every sum up to n.  Use calloc to zero them. */
    seq_t *even = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!even || !odd)
    {
        /* In case of allocation failure, return 0 (unlikely for small n). */
        if (even) free(even);
        if (odd)  free(odd);
        return (seq_t)0;
    }

    /* Base case: empty partition has 0 parts (even). */
    even[0] = (seq_t)1;

    /* Iterate over all allowed part sizes k.  Parts that are multiples of 4
       are skipped.  For each part, update the DP tables in increasing order
       of the total sum j to allow unlimited copies of the same part. */
    for (idx_t k = 1; k <= n; ++k)
    {
        if (k % 4 == 0) continue;          /* skip multiples of 4 */
        for (idx_t j = k; j <= n; ++j)
        {
            /* Adding a part of size k toggles the parity of the number of
               parts.  The existing counts for j-k are already up to date
               for this part size, so the updates below are indeed the
               standard partition DP extensions. */
            even[j] += odd[j - k];
            odd[j]  += even[j - k];
        }
    }

    /* The desired sequence value is the number of partitions of n with an
       even number of parts.  Store it before freeing the arrays. */
    seq_t result = even[n];
    free(even);
    free(odd);
    return result;
}
