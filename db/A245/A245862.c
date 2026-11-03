/*
 * A245862  Number of preferential arrangements of n labeled elements such
 *          that the minimal number of elements per rank equals 9.
 * https://oeis.org/A245862
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 18, u32 n <= 26, u64 n <= 35, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 375 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245862: Number of preferential arrangements of n labeled elements such that
   the minimal number of elements per rank equals 9.
   a(n) = A245794(n) - A245795(n) = D9[n] - D10[n]
   where Dk[n] counts ordered partitions of n with each block size ≥ k. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static void compute_D(idx_t k, idx_t target, seq_t **Dp, idx_t *cur, size_t *cap)
{
    /* Allocate / extend cache */
    ensure_cache(target, Dp, cap);
    seq_t *D = *Dp;

    /* Initialise base */
    if (*cur == 0 && D[0] == 0)
        D[0] = (seq_t)1;   /* Dk(0) = 1 */

    for (idx_t n = *cur + 1; n <= target; ++n) {
        if (n < k) {
            D[n] = (seq_t)0;
            continue;
        }
        seq_t sum = (seq_t)0;
        /* binomial(n, j) – start with j = k */
        seq_t binom = binomial((idx_t)n, (idx_t)k);
        idx_t j = k;
        for (; j <= n; ++j) {
            /* if D[n-j] is zero for many j we still multiply */
            sum += binom * D[n - j];
            if (j == n) break;
            /* update binom to binomial(n, j+1) */
            binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
        }
        D[n] = sum;
    }
    *cur = target;
}

seq_t a(idx_t n)
{
    static seq_t *D9 = NULL;
    static size_t cap9 = 0;
    static idx_t cur9 = 0;
    static seq_t *D10 = NULL;
    static size_t cap10 = 0;
    static idx_t cur10 = 0;

    /* Compute up to n for both k = 9 and k = 10 */
    compute_D(9, n, &D9, &cur9, &cap9);
    compute_D(10, n, &D10, &cur10, &cap10);

    if (n < 9)
        return (seq_t)0; /* definition starts at n = 9 */

    return D9[n] - D10[n];
}
