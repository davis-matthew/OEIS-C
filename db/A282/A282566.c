/*
 * A282566  Number of compositions (ordered partitions) of n into deficient
 *          numbers (A005100).
 * https://oeis.org/A282566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of ordered partitions of n into deficient
 *            numbers (A005100). The function uses a simple dynamic
 *            programming approach with O(n²) time and O(n) additional memory.
 *            It is correct for any n ≥ 0 and works for all provided seq_t and
 *            idx_t definitions
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* by definition a(0)=1 */
        return (seq_t)1;

    size_t sz = (size_t)n + 1;
    /* deficiency flag array: true if the number is deficient (including 1) */
    bool *def = (bool *)malloc(sz * sizeof(bool));
    /* sum of proper divisors array */
    size_t *sum = (size_t *)malloc(sz * sizeof(size_t));
    if (!def || !sum) {  /* allocation failure: return 0 as a safe value */
        free(def);
        free(sum);
        return (seq_t)0;
    }

    /* initialise sums to 0 */
    for (size_t i = 0; i < sz; ++i)
        sum[i] = 0;

    /* compute sum of proper divisors up to n */
    for (idx_t i = 1; i <= n / 2; ++i)
        for (idx_t j = i + i; j <= n; j += i)
            sum[j] += (size_t)i;

    /* classify numbers 1..n as deficient */
    for (idx_t i = 1; i <= n; ++i)
        def[i] = (sum[i] < (size_t)i);

    /* dp array for compositions: dp[k] = a(k) */
    seq_t *dp = seq_alloc_new_array(sz);
    if (!dp) {          /* allocation failure: clean up and return 0 */
        free(def);
        free(sum);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t total = (seq_t)0;
        for (idx_t d = 1; d <= k; ++d)
            if (def[d])
                total += dp[k - d];
        dp[k] = total;
    }

    seq_t result = dp[n];

    /* free temporary arrays */
    seq_free_array(dp);
    free(def);
    free(sum);

    return result;
}
