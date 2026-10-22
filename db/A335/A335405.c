/*
 * A335405  Number of integer compositions of n with product n.
 * https://oeis.org/A335405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer compositions of n whose part product
 *            equals n. The implementation enumerates all unordered
 *            factorizations of n (into integers ≥ 2). For each factorization
 *            that sums to at most n the remaining (n − sum) parts are ones;
 *            the number of ordered compositions following that fact...
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 95, u64 n <= 767,
 *            u128 n <= 8191, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b) */

static void dfs_factor(idx_t remaining, size_t start_idx,
                       idx_t *parts, size_t depth, idx_t sum_parts,
                       idx_t n_orig, const uint64_t *divs,
                       size_t div_cnt, seq_t *ans)
{
    if (remaining == 1) {
        if (sum_parts > n_orig)
            return;                     /* sum exceeds n, no composition */

        idx_t t = n_orig - sum_parts;    /* number of 1's needed */
        idx_t m = depth + t;             /* total number of parts */

        seq_t perm = (seq_t)1;
        perm *= binom(m, t);              /* positions for the 1's */
        idx_t rem = m - t;

        /* count equal non‑one factors */
        size_t i = 0;
        while (i < depth) {
            idx_t val = parts[i];
            size_t cnt = 1;
            while (i + cnt < depth && parts[i + cnt] == val)
                ++cnt;
            perm *= binom(rem, cnt);
            rem -= cnt;
            i += cnt;
        }

        *ans += perm;
        return;
    }

    for (size_t idx = start_idx; idx < div_cnt; ++idx) {
        if (divs[idx] > remaining)
            break;
        if (remaining % divs[idx] != 0)
            continue;
        parts[depth] = (idx_t)divs[idx];
        dfs_factor(remaining / divs[idx], idx, parts, depth + 1,
                   sum_parts + (idx_t)divs[idx], n_orig,
                   divs, div_cnt, ans);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* collect all divisors ≥ 2 of n in ascending order */
    uint64_t *divs = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!divs)
        exit(1);
    size_t div_cnt = 0;
    for (idx_t f = 2; f <= n; ++f)
        if (n % f == 0)
            divs[div_cnt++] = (uint64_t)f;

    idx_t parts[64];        /* depth never exceeds ~20 for n ≤ 10 000 */
    seq_t ans = (seq_t)0;
    dfs_factor(n, 0, parts, 0, 0, n, divs, div_cnt, &ans);

    free(divs);
    return ans;
}
