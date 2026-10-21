/*
 * A281776  Number of distinct topologies on an n-set that have exactly 8 open
 *          sets.
 * https://oeis.org/A281776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S(n,3) + 48*S(n,4) + 450*S(n,5) + 1800*S(n,6) +
 *            5040*S(n,7) where S(n,k) are Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;          /* cache of previously computed a(i) */
    static idx_t max_cached = 0;         /* largest argument cached so far */

    static seq_t S_prev[8] = {0};        /* S(n-1, k) values  (0 ≤ k ≤ 7) */
    static seq_t S_curr[8] = {0};        /* S(n,   k) values  (0 ≤ k ≤ 7) */

    if (!initialized) {
        initialized = true;

        /* allocate storage for a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
        max_cached = 0;

        /* initialise Stirling numbers for n = 0 */
        for (int k = 0; k < 8; ++k) S_prev[k] = (seq_t)0;
        S_prev[0] = (seq_t)1;            /* S(0,0) = 1 */
    }

    if (n <= max_cached) {
        return cache[n];
    }

    /* grow cache array if necessary */
    if (n > max_cached) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }

    /* compute a(i) for i = max_cached+1 ... n using the recurrence
       for Stirling numbers of the second kind: S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        S_curr[0] = (seq_t)0;
        for (int k = 1; k <= 7; ++k) {
            S_curr[k] = (seq_t)k * S_prev[k] + S_prev[k - 1];
        }

        /* evaluate the linear combination with the given coefficients */
        seq_t ans = S_curr[3];
        ans += (seq_t)48  * S_curr[4];
        ans += (seq_t)450  * S_curr[5];
        ans += (seq_t)1800 * S_curr[6];
        ans += (seq_t)5040 * S_curr[7];

        cache[i] = ans;

        /* prepare for the next iteration: S_prev becomes S_curr */
        for (int k = 0; k < 8; ++k) {
            S_prev[k] = S_curr[k];
        }
    }

    max_cached = n;
    return cache[n];
}
