/*
 * A298602  Expansion of (1 - x)*Product_{k>=1} (1 - x^prime(k)).
 * https://oeis.org/A298602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in (1 - x) * prod_{p prime} (1 - x^p)
 *            * Equivalent: let c(n) be the coefficient of x^n in prod_{p
 *            prime} (1 - x^p), * then a(n) = c(n) - c(n-1) (with c(-1) = 0)
 * Range:     i8 n <= 166, i16 n <= 442, i32 n <= 1331, i64 n <= 4561,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A298602, the difference between the number
 * of partitions of n into an even number of distinct prime parts (including 1)
 * and the number into an odd number of distinct prime parts.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static state: dp[i] = c(i), i.e. coefficient of x^i in prod_{p prime}(1-x^p). */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;
    static idx_t max_n = 0;          /* maximum n that has been processed */

    /* Initial allocation */
    if (dp == NULL) {
        dp_cap = 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (dp == NULL) abort();
        dp[0] = 1;
        max_n  = 0;
    }

    /* If a larger index is requested, grow the array and recompute all
     * dp[0..n] from scratch using the product over primes.  */
    if (n > max_n) {
        idx_t new_cap = n + 1;
        if (new_cap > dp_cap) {
            seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            dp = tmp;
            dp_cap = new_cap;
        }
        /* Reset all entries to zero, except dp[0] = 1 for the empty product. */
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Build list of all primes up to n.  */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto(n, &primes, &prime_cnt);

        /* For each prime p, update dp in descending order: dp[i] -= dp[i-p]. */
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            for (idx_t i = n; i >= p; --i) {
                dp[i] = dp[i] - dp[i - p];
                if (i == p) break;        /* avoid underflow of unsigned i */
            }
        }

        /* Clean up temporary prime array. */
        free(primes);
        max_n = n;
    }

    /* Compute a(n) = c(n) - c(n-1).  For n == 0, c(-1) = 0. */
    if (n == 0)
        return dp[0];
    else
        return dp[n] - dp[n - 1];
}
