/*
 * A304871  Triangle read by rows: T(n, k) gives the number of partitions
 *          (d1,d2,...,dm) of n such that k = d1/1 <= d2/2 <= ... <= dm/m for
 *          1 <= k <= n.
 * https://oeis.org/A304871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 903, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maple:  Recursively count partitions for A304871.
   Row/column mapping follows OEIS: a(n) = T(r,k) with
   r = smallest integer such that r(r+1)/2 >= n,
   k = n - r(r-1)/2.
   T(r,k) counts partitions (d1,…,dm) of r with d1 = k
   satisfying k = d1/1 ≤ d2/2 ≤ … ≤ dm/m.  The counting is
   performed by recursion with the minimal lower bound
   d_i ≥ ceil(d_{i-1} * i/(i-1)).  No memoisation is needed,
   as the branching is tiny for n ≤ 10 000.  The a() function
   keeps a static cache so repeated queries are O(1).  */

#include <stdlib.h>
#include <stdbool.h>

/* ---- recursive counting ------------------------------------------------ */

/* Lower bound for the j‑th part when the previous part
   (index j‑1) equals prev.  ceil(prev * j / (j-1)) */
static inline idx_t lower_bound(idx_t prev, idx_t j)
{
    return (prev * j + (j - 2)) / (j - 1);
}

/* Recursively count partitions of rem using part index idx
   (starting at idx = 2) with previous part value prev. */
static seq_t count_partitions(idx_t rem, idx_t idx, idx_t prev)
{
    if (rem == 0)                /* finished a partition */
        return 1;

    idx_t lb = lower_bound(prev, idx);
    if (lb > rem)                /* no feasible next part */
        return 0;

    seq_t total = 0;
    for (idx_t d = lb; d <= rem; ++d) {
        total += count_partitions(rem - d, idx + 1, d);
    }
    return total;
}

/* Count T(n,k) for n ≥ k ≥ 1.  n=0 never occurs because
   we only call with n ≥ 1, k ≥ 1. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (n == k)                   /* only one part */
        return 1;

    idx_t rem = n - k;            /* remaining sum after first part */
    return count_partitions(rem, 2, k);
}

/* ---- a(n) with row/column mapping and caching ------------------------ */

/* Compute triangular index to row number: r such that r(r-1)/2 < n ≤ r(r+1)/2 */
static idx_t row_of_index(idx_t idx)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < idx)
        ++r;
    return r;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)                 /* out of range; by convention return 0 */
        return 0;

    /* static cache of a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* enlarge cache if needed */
    if (n > cache_cap) {
        size_t new_cap = n * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;          /* allocation failure; unlikely */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute entries up to n */
    while (cache_len < n) {
        idx_t idx = cache_len + 1;          /* 1‑based index of next term */
        idx_t r = row_of_index(idx);
        idx_t prev_tri = r * (r - 1) / 2;
        idx_t k = idx - prev_tri;           /* column within row r */
        cache[cache_len] = compute_T(r, k);
        ++cache_len;
    }

    return cache[n - 1];
}
