/*
 * A360101  a(n) = Sum_{k=0..n} binomial(n+4*k-1,n-k) * Catalan(k).
 * https://oeis.org/A360101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360101
 *
 *  Formula: a(n) = Σ_{k=0..n} binomial(n+4*k-1 , n-k) * Catalan(k)
 *
 *  The implementation uses the supplied 6‑term linear recurrence
 *  (valid for n ≥ 6):
 *
 *      (n+1)a(n) = (10n-7)a(n-1)+(−19n+56)a(n-2)+(20n-90)a(n-3)
 *                  +(−15n+95)a(n-4)+(6n-49)a(n-5)+(−n+10)a(n-6)
 *
 *  Base values a(0)…a(5) are hard‑coded. Results are memoised
 *  so calls in arbitrary order are O(1) after the first evaluation.
 */

#include <stdlib.h>
#include <string.h>

/* ---------- memoisation storage ---------- */
static seq_t *cache      = NULL;   /* cache array */
static size_t cache_cap  = 0;      /* allocated size (in elements) */
static size_t cache_len  = 0;      /* highest computed index + 1 */

/* ---------- dynamic array management ---------- */
static void ensure_cap(idx_t n)
{
    /* allocate enough capacity to hold indices 0..n */
    if (cache == NULL) {
        size_t new_cap = 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        cache_cap = new_cap;
        cache_len = 0;
        return;
    }
    if ((n + 1) <= cache_cap) return;

    size_t new_cap = cache_cap << 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (p == NULL) abort();

    if (new_cap > cache_cap)
        memset(p + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = p;
    cache_cap = new_cap;
}

/* ---------- initialise known base values ---------- */
static void initialise_base(idx_t n)
{
    if (cache_len <= 0) { cache[0] = (seq_t)1; cache_len = 1; }
    if (n < 1) return;
    if (cache_len <= 1) { cache[1] = (seq_t)1; cache_len = 2; }
    if (n < 2) return;
    if (cache_len <= 2) { cache[2] = (seq_t)7;   cache_len = 3; }
    if (n < 3) return;
    if (cache_len <= 3) { cache[3] = (seq_t)40;  cache_len = 4; }
    if (n < 4) return;
    if (cache_len <= 4) { cache[4] = (seq_t)234; cache_len = 5; }
    if (n < 5) return;
    if (cache_len <= 5) { cache[5] = (seq_t)1432; cache_len = 6; }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    ensure_cap(n);
    initialise_base(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence valid only for i >= 6; base cases already handled */
        if (i <= 5) continue;

        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        int64_t coeff;

        /* (10n-7)a(n-1) */
        coeff = 10 * (int64_t)i - 7;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-1];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-1];

        /* (−19n+56)a(n-2) */
        coeff = -19 * (int64_t)i + 56;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-2];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-2];

        /* (20n-90)a(n-3) */
        coeff = 20 * (int64_t)i - 90;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-3];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-3];

        /* (−15n+95)a(n-4) */
        coeff = -15 * (int64_t)i + 95;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-4];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-4];

        /* (6n-49)a(n-5) */
        coeff = 6 * (int64_t)i - 49;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-5];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-5];

        /* (−n+10)a(n-6) */
        coeff = -1 * (int64_t)i + 10;
        if (coeff >= 0)
            sum_pos += (seq_t)coeff * cache[i-6];
        else
            sum_neg += (seq_t)(-coeff) * cache[i-6];

        seq_t numerator = sum_pos - sum_neg;
        seq_t denom     = (seq_t)(i + 1);

        cache[i] = numerator / denom;
    }

    cache_len = n + 1;
    return cache[n];
}
