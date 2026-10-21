/*
 * A284827  Expansion of Sum_{i>=1} x^prime(i)/(1 - x^prime(i)) *
 *          Product_{j>=i} 1/(1 - x^prime(j)).
 * https://oeis.org/A284827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of smallest parts in all partitions of n
 *            into prime parts
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute it by iterating over all primes p <= n in decreasing order. For each p we
 * add the contribution k * g_{>p}[n - k * p] to the result, where g_{>p}[t] counts the
 * partitions of t into primes strictly greater than p. The g array is updated after
 * each prime using the standard unbounded‑knapsack recurrence. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* Allocate arrays for g (partitions) and ans (desired sum). */
    seq_t *g = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *ans = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!g || !ans) return (seq_t)0;        /* fallback in case of OOM */

    for (idx_t i = 0; i <= n; ++i) {
        g[i] = 0;
        ans[i] = 0;
    }
    g[0] = 1;            /* one way to partition 0 (empty partition) */

    /* Get all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Process primes in descending order. */
    for (size_t pi = prime_cnt; pi-- > 0;) {
        idx_t p = primes[pi];

        /* For each residue class modulo p we compute the contribution
         * in linear time using prefix sums. */
        for (idx_t r = 0; r < p; ++r) {
            if (r > n) break;
            idx_t max_m = (n - r) / p;          /* largest m with r + m*p <= n */
            if (max_m == 0) continue;

            seq_t pref0 = 0;                    /* sum_{k=0}^{m-1} g[r + k*p] */
            seq_t pref1 = 0;                    /* sum_{k=0}^{m-1} k * g[r + k*p] */

            for (idx_t m = 0; m <= max_m; ++m) {
                idx_t pos = r + m * p;           /* current sieve position */
                seq_t g_val = g[pos];

                if (m >= 1) {
                    /* Contribution for this m:
                     * sum_{j=1}^{m} j * g[r + (m-j)*p]
                     * = m * pref0 - pref1
                     */
                    seq_t contrib = (seq_t)m * pref0 - pref1;
                    ans[pos] += contrib;
                }

                /* Update prefix sums for next iteration. */
                pref0 += g_val;
                pref1 += (seq_t)m * g_val;
            }
        }

        /* After computing contributions, update g with current prime p. */
        for (idx_t s = p; s <= n; ++s) {
            g[s] += g[s - p];
        }
    }

    free(primes);
    seq_t res = ans[n];
    free(g);
    free(ans);
    return res;
}
