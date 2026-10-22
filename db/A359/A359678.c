/*
 * A359678  Number of multisets (finite weakly increasing sequences of
 *          positive integers) with zero-based weighted sum (A359674) equal to
 *          n > 0.
 * https://oeis.org/A359678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = number of multisets of positive integers
   with zero‑based weighted sum equal to n.
   Computation: dynamic programming based on the generating function
   G.f.:  Σ_{k≥2} x^{binom(k,2)} /((1 - x^{binom(k,2)}) * Π_{j=1}^{k-1}(1 - x^{binom(k,2)-binom(j,2)}))  */

/* External linkage as required */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The function declared with external linkage */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 return 0 */
    if (n == 0) return 0;

    /* limit up to which we must compute */
    const idx_t limit = n;
    /* Allocate and zero‑initialize the cache for a(1) … a(limit) */
    seq_t *cache = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!cache) return 0;               /* memory failure */
    /* Temporary array used for the unbounded knapsack for each k */
    seq_t *cur   = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!cur) { free(cache); return 0; }

    /* Pre‑allocate a weight array large enough for all possible k
       (k ≤ √(2 limit) ≤ 200 for limit ≤ 100 000, far larger than needed) */
    idx_t w[256];
    for (idx_t k = 2; ; ++k) {
        /* binomial(k,2) = k*(k-1)/2 must fit in idx_t */
        idx_t tbinom = (k * (k - 1)) / 2;
        if (tbinom > limit) break;       /* no further k needed */

        /* Build the list of weights: one occurrence of tbinom
           and one occurrence of tbinom - binom(j,2) for j = 1 .. k-1  */
        size_t wcnt = 0;
        w[wcnt++] = tbinom;
        for (idx_t j = 1; j <= k - 1; ++j) {
            idx_t wj = tbinom - (j * (j - 1)) / 2;
            w[wcnt++] = wj;
        }

        /* Unbounded knapsack: cur[s] = number of ways to obtain sum s
           using the current list of weights (each with infinite multiplicity) */
        memset(cur, 0, (size_t)(limit + 1) * sizeof(seq_t));
        cur[0] = 1;                       /* empty combination */
        for (size_t wi = 0; wi < wcnt; ++wi) {
            idx_t wt = w[wi];
            for (idx_t s = wt; s <= limit; ++s) {
                cur[s] += cur[s - wt];
            }
        }

        /* Shift the counts by the minimal weighted sum tbinom for this k
           and add to the global cache */
        for (idx_t s = 0; s + tbinom <= limit; ++s) {
            cache[s + tbinom] += cur[s];
        }
    }

    seq_t result = cache[limit];
    free(cur);
    free(cache);
    return result;
}
