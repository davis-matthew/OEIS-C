/*
 * A392002  Number of partitions of n with parts colored by {0, 1} such that
 *          the sum of colors is congruent to 1 (mod 2).
 * https://oeis.org/A392002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with parts colored by {0,1} *
 *            such that the sum of colors is odd. Computed by dynamic
 *            programming * over part sizes. The algorithm keeps two arrays: *
 *            even[i] = number of colored-partitions of i with even color sum
 *            * odd[i] = number of colored-partitions of i with...
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *even_arr = NULL;
static seq_t *odd_arr  = NULL;
static idx_t  cache_size = 0;

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        seq_t zero = 0;
        return zero;
    }

    /* Ensure arrays are large enough and clear them */
    if (cache_size < n + 1) {
        free(even_arr);
        free(odd_arr);
        even_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        odd_arr  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_size = n + 1;
    }
    for (idx_t i = 0; i <= n; ++i) {
        even_arr[i] = 0;
        odd_arr[i]  = 0;
    }

    /* Initial condition: empty partition has even color sum (0) */
    even_arr[0] = (seq_t)1;

    /* Dynamic programming over part sizes j = 1 .. n */
    for (idx_t j = 1; j <= n; ++j) {
        /* Add a 0‑colored part of size j (parity unchanged) */
        for (idx_t k = j; k <= n; ++k) {
            even_arr[k] += even_arr[k - j];
            odd_arr[k]  += odd_arr[k - j];
        }
        /* Add a 1‑colored part of size j (parity flips) */
        for (idx_t k = j; k <= n; ++k) {
            seq_t new_even = even_arr[k] + odd_arr[k - j];
            seq_t new_odd  = odd_arr[k]  + even_arr[k - j];
            even_arr[k] = new_even;
            odd_arr[k]  = new_odd;
        }
    }

    return odd_arr[n];  /* a(n) is the odd‑parity count */
}
