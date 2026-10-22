/*
 * A326686  Sum of the third largest parts of the partitions of n into 10
 *          primes.
 * https://oeis.org/A326686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the third largest parts of the partitions of n
 *            into 10 primes
 * Range:     u8 n <= 45, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for any n >= 0. */

#include <stdlib.h>
#include <stdint.h>

/* Helper: simple sieve to generate all primes <= limit. */
static void sieve_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    if (limit < 2) { *primes_out = NULL; *cnt_out = 0; return; }
    /* Allocate a boolean array for marking composites. */
    size_t size = (size_t)(limit + 1);
    char *is_composite = (char *)calloc(size, sizeof(char));
    if (!is_composite) { *primes_out = NULL; *cnt_out = 0; return; }

    size_t pi = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            /* i is prime */
            ++pi;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_composite[(size_t)j] = 1;
                }
            }
        }
    }

    /* Allocate primes array */
    uint64_t *primes = (uint64_t *)malloc(pi * sizeof(uint64_t));
    if (!primes) { free(is_composite); *primes_out = NULL; *cnt_out = 0; return; }

    pi = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[pi++] = (uint64_t)i;
        }
    }

    free(is_composite);
    *primes_out = primes;
    *cnt_out = pi;
}

seq_t a(idx_t n) {
    /* No special handling for small n is needed; DP will return 0 automatically. */

    /* Generate all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes_upto(n, &primes, &prime_cnt);
    if (!primes || prime_cnt == 0) {
        return (seq_t)0;
    }

    /* Allocate DP tables: dp_count[k][s] and dp_sum[k][s], k = 0..10. */
    size_t max_k = 11;
    size_t sz = (size_t)n + 1;
    seq_t **dp_count = (seq_t **)malloc(max_k * sizeof(seq_t *));
    seq_t **dp_sum   = (seq_t **)malloc(max_k * sizeof(seq_t *));
    if (!dp_count || !dp_sum) {
        free(primes);
        free(dp_count); free(dp_sum);
        return (seq_t)0;
    }
    for (size_t k = 0; k < max_k; ++k) {
        dp_count[k] = (seq_t *)calloc(sz, sizeof(seq_t));
        dp_sum[k]   = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp_count[k] || !dp_sum[k]) {
            /* Clean up and return 0 on allocation failure. */
            for (size_t t = 0; t <= k; ++t) {
                free(dp_count[t]); free(dp_sum[t]);
            }
            free(dp_count); free(dp_sum);
            free(primes);
            return (seq_t)0;
        }
    }

    dp_count[0][0] = (seq_t)1;   /* One way to have 0 parts summing to 0. */

    /* Main DP: iterate over primes. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        if (p > n) break;   /* Larger primes cannot be used. */

        for (size_t k = 1; k <= 10; ++k) {
            for (idx_t s = p; s <= n; ++s) {
                idx_t prev_s = s - p;
                seq_t cnt_prev = dp_count[k-1][prev_s];
                if (cnt_prev == 0) continue;

                dp_count[k][s] += cnt_prev;

                if (k >= 8) {
                    if (k == 8) {
                        dp_sum[k][s] += (seq_t)p * cnt_prev;
                    } else { /* k > 8 */
                        dp_sum[k][s] += dp_sum[k-1][prev_s];
                    }
                }
            }
        }
    }

    seq_t result = dp_sum[10][n];

    /* Free allocated memory. */
    for (size_t k = 0; k < max_k; ++k) {
        free(dp_count[k]);
        free(dp_sum[k]);
    }
    free(dp_count);
    free(dp_sum);
    free(primes);

    return result;
}
