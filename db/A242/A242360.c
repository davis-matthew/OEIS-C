/*
 * A242360  Irregular triangular array of numerators of the positive rational
 *          numbers ordered as in Comments.
 * https://oeis.org/A242360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 2.
 * For n ≥ 3, row n contains F(n) rationals.  Given row n‑1
 * (containing rationals r₁,…,r_k) and row n‑2 (containing s₁,…,s_m),
 * form the set
 *   { r_i + 1   | i = 1..k }  ∪  { 1/(1 + s_j) | j = 1..m } .
 * Sort this set in decreasing order; that sorted list comprises
 * row n.  The sequence a(n) is the numerator of the rational
 * that appears at position n in the flattened array of rows.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct { seq_t num; seq_t den; } Rational;

/* ---------- cross‑platform allocation helpers ---------- */
#if defined(__cplusplus)
static seq_t *alloc_seq(size_t n) { return new seq_t[n]; }
static void free_seq(seq_t *p) { delete[] p; }
static Rational *alloc_rat(size_t n) { return new Rational[n]; }
static void free_rat(Rational *p) { delete[] p; }
#else
static seq_t *alloc_seq(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static void free_seq(seq_t *p) { free(p); }
static Rational *alloc_rat(size_t n) { return (Rational*)malloc(n * sizeof(Rational)); }
static void free_rat(Rational *p) { free(p); }
#endif

/* ---------- static state ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_len = 0;
static Rational *row1 = NULL, *row2 = NULL;
static size_t row1_len = 0, row2_len = 0;
static bool initialized = false;

/* ensure cache has at least 'needed' entries */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* comparison for qsort: descending by value of num/den */
static int rational_cmp(const void *pa, const void *pb)
{
    const Rational *a = (const Rational*)pa;
    const Rational *b = (const Rational*)pb;
#if defined(__cplusplus)
    if (a->num * b->den > b->num * a->den) return -1;
    if (a->num * b->den < b->num * a->den) return 1;
#else
    unsigned __int128 left  = (unsigned __int128)a->num * b->den;
    unsigned __int128 right = (unsigned __int128)b->num * a->den;
    if (left > right) return -1;
    if (left < right) return 1;
#endif
    return 0;
}

/* generate next row given previous two rows */
static void compute_next_row(void)
{
    size_t total = row1_len + row2_len;
    Rational *candidates = alloc_rat(total);

    size_t pos = 0;
    /* from row1   (x+1) */
    for (size_t i = 0; i < row1_len; ++i) {
        candidates[pos].num = row1[i].num + row1[i].den;
        candidates[pos].den = row1[i].den;
        ++pos;
    }
    /* from row2   1/(1+x) */
    for (size_t i = 0; i < row2_len; ++i) {
        candidates[pos].num = row2[i].den;
        candidates[pos].den = row2[i].den + row2[i].num;
        ++pos;
    }

    /* sort descending */
    qsort(candidates, total, sizeof(Rational), rational_cmp);

    /* create next row copy and store numerators */
    Rational *new_row = alloc_rat(total);
    for (size_t i = 0; i < total; ++i) {
        new_row[i] = candidates[i];
        ensure_cache(cache_len + 1);
        cache[cache_len++] = candidates[i].num;
    }

    free_rat(candidates);
    free_rat(row2);
    row2 = row1;
    row2_len = row1_len;
    row1 = new_row;
    row1_len = total;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise cache and first two rows */
        cache_cap = 64;
        cache = alloc_seq(cache_cap);
        cache_len = 0;
        cache[cache_len++] = (seq_t)1;
        cache[cache_len++] = (seq_t)2;

        row1 = alloc_rat(1);
        row1[0].num = (seq_t)2;
        row1[0].den = (seq_t)1;
        row1_len = 1;

        row2 = alloc_rat(1);
        row2[0].num = (seq_t)1;
        row2[0].den = (seq_t)1;
        row2_len = 1;

        initialized = true;
    }

    while (cache_len < n) {
        compute_next_row();
    }

    return cache[n - 1];
}
