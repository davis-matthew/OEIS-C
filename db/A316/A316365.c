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
 * Range:     u8 n <= 5039, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 121 terms through n = 9485 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*--------------------------------------------------------------*/
static bool is_kp(const uint64_t *f, size_t len, uint64_t total)
{
    uint64_t  distinct[20];
    uint32_t  freq[20];
    size_t    k = 0;
    for (size_t i = 0; i < len; ++i) {
        uint64_t v = f[i];
        bool found = false;
        for (size_t j = 0; j < k; ++j)
            if (distinct[j] == v) { freq[j]++; found = true; break; }
        if (!found) { distinct[k] = v; freq[k] = 1; ++k; }
    }

    /* used[s] is true iff sum s has already appeared; the sums are distinct
       and at most total, so total + 1 entries hold them (these were fixed
       arrays for n <= 10000) */
    static bool *used = NULL;
    static uint32_t *sums = NULL;
    static uint64_t room = 0;
    if (total + 1 > room) {
        room = total + 1;
        free(used);
        free(sums);
        used = (bool *)malloc(room * sizeof *used);
        sums = (uint32_t *)malloc(room * sizeof *sums);
        if (!used || !sums) abort();
    }
    for (uint64_t i = 0; i <= total; ++i) used[i] = false;
    used[0] = true;
    size_t sums_len = 1;
    sums[0] = 0;

    for (size_t d = 0; d < k; ++d) {
        uint64_t  v = distinct[d];
        uint32_t  m = freq[d];
        size_t old_len = sums_len;
        for (size_t s = 0; s < old_len; ++s) {
            uint32_t base = sums[s];
            for (uint32_t t = 1; t <= m; ++t) {
                uint32_t ns = base + t * v;
                if (ns > total) continue;          /* safety */
                if (used[ns]) return false;        /* duplicate sum */
                used[ns] = true;
                sums[sums_len++] = ns;             /* add new sum */
            }
        }
    }
    return true;
}

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
