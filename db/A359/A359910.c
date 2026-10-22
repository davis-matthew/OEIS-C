/*
 * A359910  Number of odd-length integer factorizations of n into factors > 1
 *          with the same mean as median.
 * https://oeis.org/A359910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of odd‑length factorizations of n into factors > 1 with
 *            the same mean as the median
 * Range:     u8 n <= 9683, u16 n <= 9683, u32 n <= 9683, u64 n <= 9683,
 *            u128 n <= 9683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Recursive helper: enumerate factorizations of remainder `rem`
   with factors nondecreasing starting at `start`.  `depth` is the
   current length of the partial factor list stored in `curr`.
   `sum` is the sum of the factors in `curr`.  Matching factorizations
   are counted in `cnt`. */
static void
dfs(uint64_t rem, uint64_t start, uint64_t *curr, size_t depth,
    uint64_t sum, seq_t *cnt)
{
    if (rem == 1) {
        size_t k = depth;
        if (k % 2 == 1) {                /* odd length only */
            uint64_t median = curr[k / 2];   /* curr is sorted */
            if (sum % k == 0 && sum / k == median)
                ++(*cnt);
        }
        return;
    }
    for (uint64_t f = start; f <= rem; ++f) {
        if (rem % f == 0) {
            curr[depth] = f;
            dfs(rem / f, f, curr, depth + 1, sum + f, cnt);
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) = 0 as per OEIS. */
    static uint64_t curr[64];
    seq_t cnt = 0;
    dfs((uint64_t)n, 2, curr, 0, 0, &cnt);
    return cnt;
}
