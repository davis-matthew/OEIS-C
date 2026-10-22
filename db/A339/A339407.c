/*
 * A339407  Number of partitions of n into an odd number of parts that are not
 *          multiples of 4.
 * https://oeis.org/A339407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are not
 *            multiples of 4 * and the number of parts is odd. Computed via a
 *            dynamic programming * that keeps track of partitions with even
 *            and odd part counts. * dp_even[k] = # partitions of k with even
 *            number of parts * dp_odd[k] = # partitions of k w...
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* base case, no partition has odd number of parts */

    /* Allocate DP arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *dp_even = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *dp_odd  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp_even || !dp_odd) {
        /* In a real environment we would handle allocation failure;
           here we simply return 0 to keep the signature. */
        return 0;
    }

    /* empty partition has 0 parts – even */
    dp_even[0] = 1;

    /* Iterate over all allowed part sizes v (not multiples of 4) */
    for (idx_t v = 1; v <= n; ++v) {
        if (v % 4 == 0) continue; /* skip multiples of 4 */
        for (idx_t k = v; k <= n; ++k) {
            seq_t even_from_odd = dp_odd[k - v];
            seq_t odd_from_even  = dp_even[k - v];

            /* add contributions from adding part v (parity flips) */
            dp_even[k] += even_from_odd;   /* new even from old odd */
            dp_odd[k]  += odd_from_even;   /* new odd  from old even */
        }
    }

    seq_t result = dp_odd[(size_t)n];
    free(dp_even);
    free(dp_odd);
    return result;
}
