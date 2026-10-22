/*
 * A357457  Number of partitions of n into two or more distinct odd parts.
 * https://oeis.org/A357457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of partitions of n into distinct odd parts) minus
 *            1 if n is odd (to exclude the one‑part partition)
 * Range:     u8 n <= 63, u16 n <= 164, u32 n <= 487, u64 n <= 1605,
 *            u128 n <= 5684, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) return 0;

    /* allocate a static DP array that we reuse on subsequent calls */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;           /* dp holds values for indices 0..dp_size-1 */

    /* enlarge the array if necessary */
    if (dp_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure: return 0 */
        dp = tmp;
        dp_size = n + 1;
    }

    /* clear DP array (dp[0] will be set to 1) */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* DP over odd parts only */
    for (idx_t k = 1; k <= n; k += 2) {            /* k = odd part */
        for (idx_t j = n; j >= k; --j) {           /* j = current target */
            dp[j] += dp[j - k];
            /* for unsigned j the loop terminates naturally when j < k */
        }
    }

    /* dp[n] now counts all partitions into distinct odd parts,
       including the trivial partition {n} when n is odd.    */
    seq_t result = dp[n];
    if (n & 1u)        /* subtract the one‑part partition if n is odd */
        result -= 1;

    return result;
}
