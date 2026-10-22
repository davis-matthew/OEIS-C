/*
 * A341978  Number of partitions of n into 7 distinct primes (counting 1 as a
 *          prime).
 * https://oeis.org/A341978
 *
 * Offset:    42    (a(42) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 372, u32 n <= 3484, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 7 distinct primes (counting 1 as a prime). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 7 distinct parts */
    const int K = 7;
    /* dp[k][s] : number of subsets of size k with sum s */
    static seq_t *dp = NULL;
    static size_t dp_cols = 0;          /* number of columns (n+1) */

    /* Resize dp if necessary */
    size_t needed_cols = (size_t)n + 1;
    if (needed_cols > dp_cols) {
        size_t new_cols = needed_cols;
        seq_t *new_dp = (seq_t *)realloc(dp, (K + 1) * new_cols * sizeof(seq_t));
        if (!new_dp) return 0;                     /* allocation failure */
        dp = new_dp;
        dp_cols = new_cols;
    }

    /* Clear the dp array (except dp[0][0]) */
    memset(dp, 0, (K + 1) * dp_cols * sizeof(seq_t));
    dp[0 * dp_cols + 0] = 1;                    /* dp[0][0] = 1 */

    /* Retrieve all primes up to n (excluding 1) */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((uint64_t)n, &prime_list, &prime_cnt);

    /* Helper macro to update dp */
    #define UPDATE(k, s, p) \
        dp[(k) * dp_cols + (s)] += dp[((k)-1) * dp_cols + ((s)-(p))]

    /* Process the “prime” 1 first (since 1 is treated as a prime) */
    uint64_t p_val = 1;
    for (int k = K; k >= 1; --k) {
        for (size_t s = n; s >= p_val; --s) {
            UPDATE(k, s, p_val);
            if (s == p_val) break;          /* avoid underflow of unsigned s */
        }
    }

    /* Process all actual primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        p_val = prime_list[i];
        if (p_val > (uint64_t)n) break;
        for (int k = K; k >= 1; --k) {
            for (size_t s = n; s >= p_val; --s) {
                UPDATE(k, s, p_val);
                if (s == p_val) break;
            }
        }
    }
    #undef UPDATE

    seq_t result = dp[K * dp_cols + n];

    /* Free temporary prime list */
    free(prime_list);
    return result;
}
