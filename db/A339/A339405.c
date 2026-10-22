/*
 * A339405  Number of partitions of n into an odd number of parts that are not
 *          multiples of 3.
 * https://oeis.org/A339405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into an odd number of parts that are not multiples of 3. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *even = NULL;
    static seq_t *odd  = NULL;
    static idx_t  max_n = 0;

    /* Initialize the arrays once. */
    if (even == NULL) {
        even = (seq_t *)calloc(1, sizeof(seq_t));
        odd  = (seq_t *)calloc(1, sizeof(seq_t));
        if (!even || !odd) abort();            /* allocation failed */
        even[0] = 1;                            /* empty partition: 0 parts (even) */
        max_n   = 0;
    }

    /* Recompute up to n if we haven't reached it yet. */
    if (n > max_n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp_even = (seq_t *)realloc(even, new_size * sizeof(seq_t));
        seq_t *tmp_odd  = (seq_t *)realloc(odd,  new_size * sizeof(seq_t));
        if (!tmp_even || !tmp_odd) abort();     /* allocation failed */
        even = tmp_even;
        odd  = tmp_odd;

        /* Reset the arrays for indices > 0. */
        for (idx_t i = 1; i <= n; ++i) {
            even[i] = 0;
            odd[i]  = 0;
        }
        even[0] = 1; /* base case */

        /* Dynamic programming over allowed parts */
        for (idx_t part = 1; part <= n; ++part) {
            if (part % 3 == 0) continue;         /* skip multiples of 3 */
            for (idx_t s = part; s <= n; ++s) {
                even[s] += odd[s - part];
                odd[s]  += even[s - part];
            }
        }
        max_n = n;
    }

    return odd[n];   /* partitions of n with an odd number of parts */
}
