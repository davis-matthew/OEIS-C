/*
 * A343542  Number of ways to partition n labeled elements into sets of
 *          different sizes of at least 5.
 * https://oeis.org/A343542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n labeled elements into blocks of
 *            distinct sizes all at least 5. Derived from the e.g.f. G(x) =
 *            ∏_{k≥5} (1 + x^k/k!) a(n) = n! · [x^n] G(x)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 34, u128 n <= 54,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate dynamic array for dp[0..n] */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp) return (seq_t)0;          /* allocation failure → 0 */

    dp[0] = (seq_t)1;                  /* empty set has one partition */

    if (n >= 5) {
        /* Temporary array to hold C(i,k) for the current k   */
        seq_t *comb = alloc_seq_array((size_t)n + 1);
        if (!comb) { free_seq_array(dp); return (seq_t)0; }

        for (idx_t k = 5; k <= n; ++k) {
            /* Build comb[i] = C(i,k) for all i ≥ k  */
            comb[(size_t)k] = (seq_t)1;
            for (idx_t i = k + 1; i <= n; ++i) {
                comb[(size_t)i] = comb[(size_t)(i - 1)] * (seq_t)i;
                comb[(size_t)i] = comb[(size_t)i] / (seq_t)(i - k);
            }

            /* Update dp in reverse order to avoid re‑using size k
               more than once */
            for (idx_t i = n; i >= k; --i) {
                seq_t prod = dp[(size_t)(i - k)] * comb[(size_t)i];
                dp[(size_t)i] += prod;
                if (i == k) break;                    /* avoid wrap‑around */
            }
        }
        free_seq_array(comb);
    }

    seq_t result = dp[(size_t)n];
    free_seq_array(dp);
    return result;
}
