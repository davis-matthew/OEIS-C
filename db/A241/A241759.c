/*
 * A241759  Number of partitions of n into distinct parts of the form 3^k -
 *          2^k, cf. A001047.
 * https://oeis.org/A241759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            3^k - 2^k (k ≥ 1). The generating function is ∏_{k≥1} (1 +
 *            x^(3^k-2^k))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The empty partition is the only one of size 0. */
    if (n == 0)
        return 1;

    /* dp[i] will hold the number of desired partitions of i.  We use
       a dynamic array because the sequence may require many values
       and we want to keep the computation O(n). */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp)
        return 0;        /* allocation failed – unlikely for the tested range */
    dp[0] = 1;          /* one way to form sum 0 */

    /* Generate the distinct part sizes 3^k - 2^k incrementally. */
    size_t pow3 = 3;    /* 3^1 */
    size_t pow2 = 2;    /* 2^1 */
    for (;;) {
        size_t part = pow3 - pow2;          /* distinct part size */
        if (part > N)
            break;                         /* no larger parts can be used */

        /* Update DP table for this part, iterating downward to avoid using
           the same part more than once. */
        for (size_t j = N; j >= part; --j) {
            dp[j] += dp[j - part];
        }

        /* Prepare next powers, but stop if the next multiplication would
           overflow the size_t type.  The values grow rapidly so the loop
           exits long before any overflow in the problem constraints. */
        if (pow3 > SIZE_MAX / 3 || pow2 > SIZE_MAX / 2)
            break;
        pow3 *= 3;
        pow2 *= 2;
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
