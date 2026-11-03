/*
 * A263711  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and every four consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 210, big any n
 * Validated: standard, pass, 55 terms through n = 192 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=14, a(5)=31, a(6)=34, a(7)=39, a(8)=46, a(9)=64, a(10)=104, a(11)=161, a(12)=249, a(13)=385, a(14)=561, a(15)=845, a(16)=1254, a(17)=1871, a(18)=2833 */
/* For n≤18 the values are computed by exhaustive search, for n>18 the recurrence is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*  window‑of‑4 constraint.  Only used for n≤18.                     */
static void _dfs_perm_(idx_t pos, uint64_t used, int *perm, idx_t n, seq_t *cnt)
{
    if (pos == n) {
        (*cnt)++;
        return;
    }
    int minv = (int)pos - 2;
    if (minv < 0) minv = 0;
    int maxv = (int)pos + 2;
    if (maxv > (int)n - 1) maxv = (int)n - 1;

    for (int v = minv; v <= maxv; ++v) {
        if (used & (1ULL << v))
            continue;
        perm[pos] = v;
        if (pos >= 3) {
            int mn = perm[pos], mx = perm[pos];
            for (int k = pos - 3; k <= pos; ++k) {
                if (perm[k] < mn) mn = perm[k];
                if (perm[k] > mx) mx = perm[k];
            }
            if (mx - mn > 4)
                continue;
        }
        _dfs_perm_(pos + 1, used | (1ULL << v), perm, n, cnt);
    }
}

static seq_t _count_perm_(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t cnt = 0;
    int perm[20];          /* n ≤ 18 for this routine */
    _dfs_perm_(0, 0, perm, n, &cnt);
    return cnt;
}

/* ------------------------------------------------------------------ */
/*  Enumerate permutations 0..n-1 satisfying the bandwidth ≤2   and   */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* largest n already computed */

    if (n > cache_max) {
        size_t new_size = (size_t)(n + 1);        /* 1‑based indexing */
        if (cache == NULL) {
            cache = mem_new(seq_t, new_size);
        } else {
            cache = mem_resize(seq_t, cache, new_size);
        }
        if (cache == NULL)
            abort();           /* memory exhausted */

        /* Initialise newly allocated slots */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 18) {
                cache[i] = _count_perm_(i);
            } else {
                /* Empirical recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7) */
                seq_t v = cache[i - 1];
                v += cache[i - 3];
                v -= cache[i - 4];
                v += cache[i - 5] * (seq_t)2;
                v -= cache[i - 6];
                v += cache[i - 7];
                cache[i] = v;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
