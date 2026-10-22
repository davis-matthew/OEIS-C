/*
 * A347797  Number of compositions (ordered partitions) of n into at most 4
 *          nonprime parts.
 * https://oeis.org/A347797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A347797: Number of compositions of n into at most 4 nonprime parts.
   Computation uses dynamic programming with O(n·π(n)) operations per call,
   where π(n) is the number of primes ≤ n.  For n up to about 10000 this runs
   well under the time limit. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void sieve_primes(idx_t n, idx_t **primes_out, size_t *cnt_out) {
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime) return; /* allocation failure, caller may handle */
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = (idx_t)(i * i); j <= n; j += i)
                is_prime[j] = false;
        }
    }
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) ++cnt;
    *primes_out = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!*primes_out) { free(is_prime); return; }
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) (*primes_out)[idx++] = i;
    *cnt_out = cnt;
    free(is_prime);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1; /* the empty composition */

    /* Allocate working arrays of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *prefix = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_prev || !dp_curr || !prefix) {
        /* Allocation failure – return 0 as a safe fallback */
        free(dp_prev); free(dp_curr); free(prefix);
        return (seq_t)0;
    }

    dp_prev[0] = (seq_t)1;          /* f(0,0)=1, others 0 */
    seq_t result = (seq_t)0;        /* will accumulate f(k,n) over k=0..4 */

    /* Prepare list of primes up to n once for all k */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(n, &primes, &prime_cnt);

    /* k = 0 part (empty composition) only contributes when n==0 */
    /* For n>0 dp_prev[n]==0, so nothing to add */

    for (idx_t k = 1; k <= 4; ++k) {
        /* Compute prefix sums of dp_prev */
        prefix[0] = dp_prev[0];
        for (idx_t i = 1; i <= n; ++i)
            prefix[i] = prefix[i - 1] + dp_prev[i];

        /* Initialize dp_curr[i] = prefix[i-1] for i>=1, dp_curr[0]=0 */
        dp_curr[0] = (seq_t)0;
        for (idx_t i = 1; i <= n; ++i)
            dp_curr[i] = prefix[i - 1];

        /* Subtract contributions of prime parts */
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = primes[pi];
            for (idx_t i = p; i <= n; ++i)
                dp_curr[i] -= dp_prev[i - p];
        }

        /* Add result for length k */
        result += dp_curr[n];

        /* Swap dp_prev and dp_curr for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Clean up */
    free(dp_prev);
    free(dp_curr);
    free(prefix);
    free(primes);

    /* Include k=0 part for n==0 (handled earlier) */
    return result;
}
