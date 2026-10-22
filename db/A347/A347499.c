/*
 * A347499  Triangle read by rows: n-th row is the lexicographically earliest
 *          n-element subset of {1,2,3,...,A347498(n)} with the property that
 *          all products i * j are distinct for i <= j.
 * https://oeis.org/A347499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = lexicographically earliest entry of the triangle where
 *            the r‑th row is the minimal n‑element subset of {1,…,M} with
 *            distinct pairwise products for i ≤ j
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347499 */

/*--------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--------------------------------------------------------------*/
/* Bitset helpers for products 0 .. M*M  (idx_t is uint64_t)   */
static inline bool test_bit(const uint8_t *bs, idx_t pos)
{
    return (bs[pos >> 3] & (1U << (pos & 7))) != 0;
}
static inline void set_bit(uint8_t *bs, idx_t pos)
{
    bs[pos >> 3] |= (1U << (pos & 7));
}
static inline void clear_bit(uint8_t *bs, idx_t pos)
{
    bs[pos >> 3] &= ~(1U << (pos & 7));
}

/*--------------------------------------------------------------*/
/*  Depth‑first search for a lexicographically earliest subset. */
static bool dfs(idx_t start, size_t depth, size_t r,
                idx_t *sub, uint8_t *bits, idx_t M)
{
    if (depth == r) return true;

    for (idx_t cand = start; cand <= M; ++cand)
    {
        bool conflict = false;
        for (size_t j = 0; j < depth && !conflict; ++j)
        {
            idx_t prod = sub[j] * cand;
            if (test_bit(bits, prod)) conflict = true;
        }
        if (conflict) continue;
        idx_t sq = cand * cand;
        if (test_bit(bits, sq)) continue;

        /* accept cand */
        sub[depth] = cand;
        for (size_t j = 0; j < depth; ++j)
            set_bit(bits, sub[j] * cand);
        set_bit(bits, sq);

        if (dfs(cand + 1, depth + 1, r, sub, bits, M)) return true;

        /* backtrack */
        for (size_t j = 0; j < depth; ++j)
            clear_bit(bits, sub[j] * cand);
        clear_bit(bits, sq);
    }
    return false;
}

/*--------------------------------------------------------------*/
/*  Search for existence of a subset, optionally return it.   */
static bool search_solution(idx_t M, size_t r, idx_t *out)
{
    size_t bitcnt = (size_t)M * (size_t)M + 1;
    size_t bytes   = (bitcnt + 7) >> 3;
    uint8_t *bits = (uint8_t*)calloc(bytes, 1);
    if (!bits) return false;

    idx_t *sub = (idx_t*)malloc(r * sizeof(idx_t));
    if (!sub) { free(bits); return false; }

    bool found = dfs(1, 0, r, sub, bits, M);

    if (found && out)
    {
        for (size_t i = 0; i < r; ++i) out[i] = sub[i];
    }

    free(sub);
    free(bits);
    return found;
}

/*--------------------------------------------------------------*/
/*  Cache for computed rows                                 */
static seq_t **row_cache = NULL;   /* array of pointers to seq_t[r] */
static size_t *row_len   = NULL;   /* length of each cached row   */
static size_t cache_size = 0;

/*--------------------------------------------------------------*/
static void ensure_cache(size_t r)
{
    if (r < cache_size) return;

    size_t new_size = cache_size == 0 ? 4 : cache_size * 2;
    while (new_size < r) new_size <<= 1;

    seq_t **new_cache = (seq_t**)realloc(row_cache,
                                         (new_size + 1) * sizeof(seq_t*));
    size_t *new_len   = (size_t*)realloc(row_len,
                                        (new_size + 1) * sizeof(size_t));
    if (!new_cache || !new_len) return; /* allocation failure */

    for (size_t i = cache_size; i <= new_size; ++i) {
        new_cache[i] = NULL;
        new_len[i]   = 0;
    }
    row_cache = new_cache;
    row_len   = new_len;
    cache_size = new_size;
}

/*--------------------------------------------------------------*/
/*  Compute row r (1‑based) and cache it.  Return pointer to row. */
static seq_t *get_row(size_t r)
{
    ensure_cache(r);
    if (row_cache[r]) return row_cache[r];

    /* Find minimal M by exponential growth and binary search */
    idx_t low  = r, high = r;
    while (!search_solution(high, r, NULL)) high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (search_solution(mid, r, NULL))
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t M = low;

    /* Allocate space for the found subset */
    idx_t *subset_idx = (idx_t*)malloc(r * sizeof(idx_t));
    if (!subset_idx) return NULL;
    if (!search_solution(M, r, subset_idx)) {
        free(subset_idx);
        return NULL;
    }

    /* Convert to seq_t for caching */
    seq_t *subset_seq = (seq_t*)malloc(r * sizeof(seq_t));
    if (!subset_seq) { free(subset_idx); return NULL; }
    for (size_t i = 0; i < r; ++i)
        subset_seq[i] = (seq_t)subset_idx[i];

    free(subset_idx);
    row_cache[r] = subset_seq;
    row_len[r]   = r;
    return subset_seq;
}

/*--------------------------------------------------------------*/
/*  Public sequence function                                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* out of offset */

    /* Find the row containing the n‑th term */
    idx_t r = 1;
    idx_t sum = 1;                           /* triangular number T(r) */
    while (n > sum) {
        ++r;
        sum = r * (r + 1) / 2;
    }

    idx_t prev = (r - 1) * r / 2;
    idx_t pos  = n - prev - 1;               /* zero‑based position */

    seq_t *row = get_row((size_t)r);
    if (!row) return (seq_t)0;
    return row[pos];
}
