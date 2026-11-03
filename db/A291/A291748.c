/*
 * A291748  Expansion of Product_{k>=1} (1 + x^(2*k^2 - 1)).
 * https://oeis.org/A291748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^(2k^2-1))
 * Range:     u8 n <= 1423, u16 n <= 5079, u32 n <= 21911, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 103 terms through n = 9361 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (1 + x^(2k^2-1)) expands to a subset‑sum count over the set {1,7,17,31,…} */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array dp[0..n] and initialise to 0 */
    seq_t *dp = alloc_seq(n + 1);
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;   /* empty subset sums to 0 */

    /* For each exponent d = 2*k^2 - 1 ≤ n, add its contribution */
    for (idx_t k = 1;; ++k) {
        idx_t d = 2 * k * k - 1;
        if (d > n) {
            break;
        }
        for (idx_t s = n; s >= d; --s) {
            dp[s] = dp[s] + dp[s - d];
        }
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
