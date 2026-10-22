/*
 * A310031  Coordination sequence Gal.5.39.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   allocate_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *  a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5)
 *        + a(n-13) - a(n-14) + a(n-15) - a(n-16) + a(n-17) - a(n-18)
 *  for n > 18
 * Initial values (n = 0 … 23):
 *  1, 3, 5, 8, 12, 16, 19, 20, 21, 24, 28, 32, 36, 39,
 *  40, 41, 44, 49, 55, 58, 57, 57, 60, 65
 */

#include <stdlib.h>
#include "../utils/allocate_seq.h"

static const seq_t init_vals[24] = {
    1, 3, 5, 8, 12, 16, 19, 20, 21, 24,
    28, 32, 36, 39, 40, 41, 44, 49, 55, 58,
    57, 57, 60, 65
};

static inline void free_seq(seq_t* ptr) {
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

/* Return a(n) for OEIS sequence A310031 */
seq_t a(idx_t n)
{
    if (n < 24) return init_vals[n];

    /* Allocate a temporary array large enough to hold up to the requested index.
     * For n up to about 10 000 this allocation is trivial in memory. */
    seq_t *dp = allocate_seq(n + 1);
    if (dp == NULL) return (seq_t)0;   /* allocation failed – return 0 as a safe default */

    /* Copy the known initial values. */
    for (idx_t i = 0; i < 24; ++i) dp[i] = init_vals[i];

    /* Compute subsequent values using the recurrence. */
    for (idx_t i = 24; i <= n; ++i) {
        dp[i] = dp[i-1] - dp[i-2] + dp[i-3] - dp[i-4] + dp[i-5];
        dp[i] += dp[i-13] - dp[i-14] + dp[i-15] - dp[i-16] + dp[i-17] - dp[i-18];
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
