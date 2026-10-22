/*
 * A366841  Least positive integer whose odd prime factors sum to n, starting
 *          with n = 5.
 * https://oeis.org/A366841
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer whose odd prime factors sum to n
 * Range:     u8 n <= 17, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, build_primes_upto, cmp_seq_t, free_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Compute a(n) for any n >= 5 (behaviour for n < 5 is unused). */

    idx_t max_n = n;          /* upper bound for DP */
    size_t size   = (size_t)max_n + 1;

    /* DP array: dp[s] stores the minimal product for sum = s */
    seq_t *dp = alloc_seq(size);
    for (size_t i = 0; i < size; ++i) dp[i] = 0;
    dp[0] = 1;                /* product for sum 0 */

    /* Generate all primes up to max_n */
    uint64_t *primes = NULL;
    size_t    primes_cnt = 0;
    build_primes_upto(max_n, &primes, &primes_cnt);

    /* Unbounded knapsack over odd primes, minimizing product */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p == 2) continue;                     /* skip even prime 2 */
        size_t sp = (size_t)p;
        for (size_t s = sp; s < size; ++s) {
            if (dp[s - sp] == 0) continue;       /* no representation yet */
            seq_t cand = seq_mul(dp[s - sp], (seq_t)p);
            if (dp[s] == 0 || cmp_seq_t(&cand, &dp[s]) < 0) {
                dp[s] = cand;
            }
        }
    }

    free(primes);            /* free temporary prime list */

    seq_t result = dp[max_n];
    free_seq(dp);            /* free DP array memory */
    return result;
}
