/*
 * A338436  a(n) = n! * [x^n] Product_{k=1..n, gcd(n,k) = 1} 1 / (1 - x^k/k!).
 * https://oeis.org/A338436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * [x^n] ∏_{k=1..n, gcd(n,k)=1} 1 / (1 - x^k/k!)
 * The implementation uses dynamic programming over the allowed part sizes.
 * For each k coprime to n the infinite series 1/(1‑x^k/k!) is expanded
 * into powers of x^k.  Because every term of the form n! / ∏k!^m is an integer,
 * intermediate divisions are performed exactly with integer arithmetic.
 *
 * Complexity: O(∑_{k≤n,gcd(n,k)=1} n/k) ≈ O(n^2) in the worst case.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factorial.h"   /* factorial(idx_t n) returns seq_t */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate two working arrays of size n+1.  */
    size_t len = (size_t)(n + 1);
    seq_t *dp   = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return (seq_t)0;      /* fallback on allocation failure */
    }

    seq_t fact_n = factorial(n);
    dp[0] = fact_n;
    memset(dp + 1, 0, (len - 1) * sizeof(seq_t));

    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_idx(n, k) != 1) continue;

        seq_t fact_k = factorial(k);
        memset(next, 0, len * sizeof(seq_t));

        for (idx_t s = 0; s <= n; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;

            /* Term with m = 0 (no copy of k).  */
            next[s] = next[s] + val;

            /* Add contributions for m ≥ 1.  */
            seq_t mult = val / fact_k;          /* val / fact_k^1 */
            idx_t pos = s + k;
            while (pos <= n) {
                next[pos] = next[pos] + mult;
                /* Prepare for next power: divide by fact_k again.  */
                mult = mult / fact_k;           /* now mult = val / fact_k^2, etc. */
                pos += k;
            }
        }
        /* Swap dp and next for next iteration. */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[n];

    free(dp); free(next);
    return result;
}
