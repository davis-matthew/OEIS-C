/*
 * A304632  Expansion of (1/(1 - x))* Product_{k>=1} (1 + x^k)/(1 + x^(3*k)).
 * https://oeis.org/A304632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(k), where p(k) is the number of partitions
 *            of k into parts congruent to 1 or 5 modulo 6
 * Range:     u8 n <= 24, u16 n <= 80, u32 n <= 277, u64 n <= 1015,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *values = NULL;          /* cumulative sums a(k) */
    static idx_t  size   = 0;             /* number of stored values */
    static idx_t  capacity = 0;           /* allocated size of values */

    /* If enough values already computed, return directly. */
    if (n < size)
        return values[n];

    /* Compute values up to index n.  Grow array to next power of two. */
    idx_t new_cap = (capacity == 0) ? 16 : capacity;
    while (new_cap <= n)
        new_cap *= 2;

    /* Allocate a new array for the cumulative sums. */
    seq_t *new_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_vals)
        return 0;          /* out of memory, best effort */

    /* Temporary array for partition counts p(k). */
    idx_t N = new_cap - 1;                     /* maximum index needed */
    seq_t *dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp) {                                 /* out of memory */
        free(new_vals);
        return 0;
    }

    dp[0] = 1;                                 /* empty partition */

    /* Enumerate allowed parts: m ≡ 1 or 5 (mod 6). */
    for (idx_t m = 1; m <= N; ++m) {
        if (m % 6 == 1 || m % 6 == 5) {
            for (idx_t i = m; i <= N; ++i)
                dp[i] += dp[i - m];
        }
    }

    /* Compute cumulative sums a(k) = sum_{t=0..k} p(t). */
    seq_t sum = 0;
    for (idx_t i = 0; i <= N; ++i) {
        sum += dp[i];
        new_vals[i] = sum;
    }

    /* Clean up and update the static table. */
    free(dp);
    free(values);
    values   = new_vals;
    size     = new_cap;
    capacity = new_cap;

    return values[n];
}
