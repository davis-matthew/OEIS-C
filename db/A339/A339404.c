/*
 * A339404  Number of partitions of n into an even number of parts that are
 *          not multiples of 3.
 * https://oeis.org/A339404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of parts
 *            that are not multiples of 3. Computation by dynamic programming
 *            tracking parity of part counts. dp_even[s] – number of
 *            partitions of s using only parts not divisible by 3 and having
 *            an even number of parts. dp_odd[s] – same but with an...
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;            /* array size */
    /* Allocate arrays for even and odd part counts */
    seq_t *dp_even = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *dp_odd  = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp_even || !dp_odd) {
        /* In an OOM situation, return 0 */
        free(dp_even);
        free(dp_odd);
        return 0;
    }

    /* Initialize arrays */
    for (size_t i = 0; i <= N; ++i) {
        dp_even[i] = 0;
        dp_odd[i]  = 0;
    }
    dp_even[0] = 1;   /* empty partition: 0 parts (even) */

    /* Dynamic programming over allowed parts (not multiples of 3) */
    for (idx_t k = 1; k <= n; ++k) {
        if (k % 3 == 0) continue;          /* skip multiples of 3 */
        size_t sk = (size_t)k;
        for (size_t s = sk; s <= N; ++s) {
            dp_even[s] += dp_odd[s - sk];
            dp_odd[s]  += dp_even[s - sk];
        }
    }

    seq_t result = dp_even[N];

    free(dp_even);
    free(dp_odd);
    return result;
}
