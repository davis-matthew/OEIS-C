/*
 * A316365  Number of factorizations of n into factors > 1 such that every
 *          distinct subset of the factors has a different sum.
 * https://oeis.org/A316365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1
 *            such that every distinct sub‑multiset of the factors has a
 *            different sum (knapsack partitions)
 * Range:     u8 n <= 5039, u16 n <= 9485, u32 n <= 9485, u64 n <= 9485,
 *            u128 n <= 9485, big any n
 * Helpers:   is_kp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_kp.h"

/*--------------------------------------------------------------*/
/* Helper: test whether a given factorization satisfies the
   distinct subset‑sum property.                                 */

/*--------------------------------------------------------------*/
/* Recursive enumeration of all unordered factorizations of rem
   into factors ≥ min_factor.                                     */
/*--------------------------------------------------------------*/
static void dfs(uint64_t rem, uint64_t min_factor,
                uint64_t *curr, size_t idx,
                seq_t *cnt, uint64_t total)
{
    /* Take the remaining product as the last factor */
    if (rem >= min_factor) {
        curr[idx] = rem;
        if (is_kp(curr, idx + 1, total)) (*cnt)++;
    }

    /* Try to split rem further */
    for (uint64_t f = min_factor; f * f <= rem; ++f) {
        if (rem % f == 0) {
            curr[idx]       = f;
            dfs(rem / f, f, curr, idx + 1, cnt, total);
        }
    }
}

/*--------------------------------------------------------------*/
/* Main sequence function                                     */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    if (n == 1) return 1;         /* special case */

    uint64_t N = (uint64_t)n;
    seq_t    ans = 0;
    uint64_t curr[32];            /* depth will never exceed 32 for n <= 10000 */

    dfs(N, 2, curr, 0, &ans, N);
    return ans;
}
