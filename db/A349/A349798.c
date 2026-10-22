/*
 * A349798  Number of weakly alternating ordered prime factorizations of n
 *          with at least two adjacent equal parts.
 * https://oeis.org/A349798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (# weakly alternating ordered prime factorizations of
 *            n) * – (# strongly alternating ordered prime factorizations of
 *            n) * The function enumerates all permutations of the multiset of
 *            prime * factors of n and counts those that satisfy the weak (≤
 *            or ≥) * alternating pattern and those that...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper that counts permutations starting from a partial
 * permutation (prev element already chosen).  "dir" tells whether
 * the next comparison must be ≤ (dir == 0) or ≥ (dir == 1) for weak
 * permutations, or < / > for strict permutations when "strict" is true.
 * "remaining" is the number of elements yet to place. */
static seq_t dfs_rec(bool strict, int k, const uint64_t *pr,
                     int *cnt, int prev, int dir, int remaining)
{
    if (remaining == 0)
        return 1;                 /* a complete permutation */

    seq_t total = 0;
    for (int j = 0; j < k; ++j) {
        if (cnt[j] == 0)
            continue;

        /* For strict alternation disallow equal adjacent elements. */
        if (strict && j == prev)
            continue;

        /* Determine if the next element satisfies the required
         * inequality for the current direction. */
        if (strict) {
            if (dir == 0) {             /* need prev < next */
                if (pr[j] <= pr[prev])
                    continue;
            } else {                    /* need prev > next */
                if (pr[j] >= pr[prev])
                    continue;
            }
        } else { /* weak alternation allowing equality */
            if (dir == 0) {             /* need prev ≤ next */
                if (pr[j] < pr[prev])
                    continue;
            } else {                    /* need prev ≥ next */
                if (pr[j] > pr[prev])
                    continue;
            }
        }

        cnt[j]--;                              /* use this prime */
        total += dfs_rec(strict, k, pr, cnt,
                         j, dir ^ 1, remaining - 1);
        cnt[j]++;                              /* backtrack */
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* a(1) = 0 */

    /* Factorise n.  The problem limits n ≤ 10 000, so a 32‑bit array
     * is sufficient for the primes and their exponents. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t ksize;
    factor_u64(n, primes, exps, &ksize);
    int k = (int)ksize;
    if (k == 0)
        return 0;   /* n = 1 */

    /* Store the counts of each prime factor. */
    int cnt[32];
    int total = 0;
    for (int i = 0; i < k; ++i) {
        cnt[i] = (int)exps[i];
        total += cnt[i];
    }
    if (total <= 1)
        return 0;   /* at least two parts required for adjacency */

    /* Count weakly alternating permutations. */
    seq_t weak_total = 0;
    for (int i = 0; i < k; ++i) {
        if (cnt[i] == 0)
            continue;
        cnt[i]--;                         /* choose first element */
        int rem = total - 1;
        weak_total += dfs_rec(false, k, primes, cnt,
                              i, 0, rem);   /* start with ≤ */
        weak_total += dfs_rec(false, k, primes, cnt,
                              i, 1, rem);   /* start with ≥ */
        cnt[i]++;                         /* restore */
    }
    /* For k == 1 (all primes equal) a single permutation has been
     * counted twice (once for each direction); subtract the duplicate. */
    if (k == 1)
        weak_total -= 1;

    /* Count strictly (strongly) alternating permutations. */
    seq_t strong_total = 0;
    for (int i = 0; i < k; ++i) {
        if (cnt[i] == 0)
            continue;
        cnt[i]--;
        int rem = total - 1;
        strong_total += dfs_rec(true, k, primes, cnt,
                                i, 0, rem);   /* start with < */
        strong_total += dfs_rec(true, k, primes, cnt,
                                i, 1, rem);   /* start with > */
        cnt[i]++;
    }

    return weak_total - strong_total;
}
