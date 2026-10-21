/*
 * A290807  Number of partitions of n into Pell parts (A000129).
 * https://oeis.org/A290807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of partitions of n into Pell numbers (A000129)  
    Computed with standard coin‑change DP.  */
#include <stddef.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;                 /* a(0) = 1 by definition */
    }

    /* Allocate DP array of length n+1, initialized to 0. */
    size_t len = (size_t)n + 1;
    seq_t *dp = seq_alloc_new_array(len);
    if (!dp) return 0;           /* allocation failure – return 0 */

    /* Initialize DP table.  All newly allocated seq_t are 0 by default,
       but we set them explicitly for safety. */
    for (size_t i = 1; i < len; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;                    /* empty partition */

    /* Generate all Pell numbers <= n and store them in a small array. */
    seq_t parts[64];              /* more than enough for n <= 10000 */
    size_t parts_cnt = 0;

    if ((seq_t)n >= 1) parts[parts_cnt++] = 1;
    if ((seq_t)n >= 2) parts[parts_cnt++] = 2;

    seq_t p0 = 1, p1 = 2;
    while (1) {
        seq_t p_next = 2 * p1 + p0;          /* Pell recurrence */
        if (p_next > (seq_t)n) break;
        parts[parts_cnt++] = p_next;
        p0 = p1;
        p1 = p_next;
    }

    /* DP over the Pell parts (unlimited supply). */
    for (size_t ci = 0; ci < parts_cnt; ++ci) {
        size_t val = (size_t)parts[ci];
        for (size_t i = val; i < len; ++i) {
            dp[i] += dp[i - val];
        }
    }

    seq_t result = dp[len - 1];
    seq_free_array(dp);
    return result;
}
