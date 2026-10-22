/*
 * A347743  Number of compositions (ordered partitions) of n into at most 6
 *          prime parts.
 * https://oeis.org/A347743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..6} A121303(n,k) for n ≥ 2 (number of
 *            compositions of n into at most 6 prime parts). Algorithm:
 *            dynamic programming by successive convolution with primes
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- helper to generate all primes ≤ limit --------------------------------*/
static void generate_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    size_t lim = (size_t)limit;
    uint8_t *is_prime = (uint8_t *)calloc(lim + 1, sizeof(uint8_t));
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    for (size_t i = 2; i <= lim; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;
        }
    }
    /* Count primes */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) ++prime_cnt;
    uint64_t *pr = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!pr) { free(is_prime); *out = NULL; *cnt = 0; return; }
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) pr[idx++] = (uint64_t)i;
    free(is_prime);
    *out = pr;
    *cnt = prime_cnt;
}

/*--- main sequence function ----------------------------------------------*/
seq_t a(idx_t n)
{
    /* Handle offset explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n < 6) {
        /* For n < 6, compositions can include at most n primes (each ≥2),
           but we still use the general DP; no special shortcut needed. */
    }

    /* Generate primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes_upto(n, &primes, &prime_cnt);
    if (prime_cnt == 0 || primes == NULL) return (seq_t)0; /* no primes <= n */

    idx_t len = n + 1;
    /* Allocate DP arrays */
    seq_t *prev = (seq_t *)calloc((size_t)len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)len, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev); free(curr); free(primes);
        return (seq_t)0;
    }

    prev[0] = (seq_t)1;  /* dp[0][0] = 1 */
    seq_t total = (seq_t)0;

    for (size_t k = 1; k <= 6; ++k) {
        /* zero curr (already zero from calloc, but zero on reuse) */
        for (idx_t i = 0; i <= n; ++i) curr[i] = (seq_t)0;

        /* convolution with each prime */
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            if (p > n) break;
            for (idx_t i = p; i <= n; ++i) {
                curr[i] += prev[i - p];
            }
        }

        total += curr[n];  /* accumulate for this k */
        /* swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    free(primes);
    return total;
}
