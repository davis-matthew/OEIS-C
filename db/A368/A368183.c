/*
 * A368183  Number of sets that can be obtained by choosing a different binary
 *          index of each binary index of n.
 * https://oeis.org/A368183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   binary_indices
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of distinct sets of binary indices chosen
   from each binary index of n such that all chosen indices are different.
   The function enumerates all subsets of the union of binary indices of the
   binary indices, checks whether a perfect matching exists, and counts
   the subsets for which it does.  This works for any 64‑bit n and
   is fast for n up to about 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binary_indices.h"

/* depth‑first search that tries to match the first `idx` left vertices
   using currently used right vertices `used`.  `S_mask` contains the
   allowed right vertices (the chosen subset).  For each left vertex
   the list of admissible right vertices is in `R[idx]`. */
static bool dfs_rec(size_t idx, uint64_t used, uint64_t S_mask,
                    size_t k, const int *R_cnt, const int (*R)[64])
{
    if (idx == k)
        return true;                       /* all left vertices matched */

    for (int t = 0; t < R_cnt[idx]; ++t) {
        int r = R[idx][t];                 /* candidate right vertex (1‑based) */
        uint64_t bit = 1ULL << (r - 1);
        if ((S_mask & bit) && !(used & bit)) {
            if (dfs_rec(idx + 1, used | bit, S_mask,
                        k, R_cnt, R))
                return true;
        }
    }
    return false;
}

static bool has_matching(uint64_t S_mask, size_t k,
                         const int *R_cnt, const int (*R)[64])
{
    return dfs_rec(0, 0, S_mask, k, R_cnt, R);
}

seq_t a(idx_t n)
{
    idx_t j_list[64];
    size_t k;
    binary_indices(n, j_list, &k);

    /* n==0 has no binary indices → only the empty set */
    if (k == 0)
        return (seq_t)1;

    int R_vals[64][64];
    int R_cnt[64] = {0};
    bool union_set[65] = {0};                /* positions 1..64 */

    for (size_t i = 0; i < k; ++i) {
        idx_t j = j_list[i];
        idx_t pos = 1;
        idx_t temp = j;
        while (temp) {
            if (temp & 1ULL) {
                int r = (int)pos;                /* binary index of j */
                R_vals[i][R_cnt[i]++] = r;
                union_set[r] = true;
            }
            temp >>= 1;
            ++pos;
        }
    }

    int U_pos[65];
    size_t m = 0;
    for (int r = 1; r <= 64; ++r)
        if (union_set[r])
            U_pos[m++] = r;

    /* If we have more left vertices than distinct right ones,
       a perfect matching is impossible. */
    if (k > m)
        return (seq_t)0;

    seq_t count = 0;

    /* Enumerate all subsets of the union of indices of size exactly k. */
    uint64_t upper = 1ULL << m;                /* m <= 7, so safe */
    for (uint64_t sub = 0; sub < upper; ++sub) {
        if (__builtin_popcountll(sub) != k)
            continue;
        uint64_t S_mask = 0;
        for (size_t i = 0; i < m; ++i)
            if (sub & (1ULL << i))
                S_mask |= 1ULL << (U_pos[i] - 1);
        if (has_matching(S_mask, k, R_cnt, R_vals))
            ++count;
    }

    return count;
}
