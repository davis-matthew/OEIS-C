/*
 * A301331  Number of compositions (ordered partitions) of n into parts having
 *          the same number of divisors as n.
 * https://oeis.org/A301331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into parts having the same
 *            number of divisors as n
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 60, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"

/* Helper structure: a list of numbers that share the same divisor count. */
typedef struct {
    size_t len;      /* current number of elements */
    size_t cap;      /* current capacity of arr */
    idx_t *arr;      /* dynamic array of numbers */
} Group;

/* Compute the divisor count of a number using its smallest‑prime‑factor array. */
static idx_t divisor_count(idx_t num, const idx_t *spf) {
    idx_t cnt = 1;
    while (num > 1) {
        idx_t p = spf[num];
        idx_t e = 0;
        while (num % p == 0) {
            num /= p;
            ++e;
        }
        cnt *= (e + 1);
    }
    return cnt;
}

/* Static state shared by all calls to a(idx_t n). */
static idx_t          max_n       = 0;
static idx_t        *divcnt      = NULL;   /* divisor counts up to max_n */
static size_t        divcnt_cap  = 0;
static idx_t        *spf         = NULL;   /* smallest prime factor array */
static idx_t          spf_limit   = 0;
static Group       **groups      = NULL;   /* groups[d] contains all numbers <=max_n with d(k)=d */
static size_t        groups_cap  = 0;
static seq_t        *cache_arr   = NULL;   /* memoized a(n) values */
static uint8_t      *cached_flag = NULL;   /* 0=not computed, 1=computed */
static size_t        cache_cap   = 0;

/* Ensure that divcnt, spf, groups, cache_arr and cached_flag are all allocated
   at least up to indices ‘up_to’.  Grow the arrays and recompute divisor counts
   for the new range. */
static void ensure_precompute(idx_t up_to) {
    if (up_to <= max_n) return;

    /* enlarge divisor‑count array if necessary */
    if (divcnt_cap <= up_to) {
        size_t new_cap = (size_t)up_to + 1;
        idx_t *tmp = (idx_t *)realloc(divcnt, new_cap * sizeof(idx_t));
        divcnt = tmp;
        divcnt_cap = new_cap;
    }

    /* enlarge groups pointer array if necessary */
    if (groups_cap == 0) {
        groups_cap = 8;
        groups = (Group **)malloc(groups_cap * sizeof(Group *));
        for (size_t i = 0; i < groups_cap; ++i) groups[i] = NULL;
    }

    /* enlarge cache and flag arrays */
    if (cache_cap <= up_to) {
        size_t new_cap = (size_t)up_to + 1;
        seq_t *tmp = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
        cache_arr   = tmp;
        uint8_t *tmpf = (uint8_t *)realloc(cached_flag, new_cap * sizeof(uint8_t));
        cached_flag = tmpf;
        for (size_t i = max_n + 1; i <= up_to; ++i) cached_flag[i] = 0;
        cache_cap = new_cap;
    }

    /* rebuild or extend the SPF table if the requested limit is larger */
    if (!spf || spf_limit < up_to) {
        if (spf) free(spf);
        build_spf(up_to, &spf);   /* build_spf updates spf array */
        spf_limit = up_to;
    }

    /* compute divisor counts for new numbers and add them to the appropriate group */
    for (idx_t i = max_n + 1; i <= up_to; ++i) {
        idx_t d = divisor_count(i, spf);
        divcnt[i] = d;

        /* ensure groups array is large enough to index by d */
        if ((size_t)d >= groups_cap) {
            size_t new_cap = groups_cap ? groups_cap << 1 : 8;
            while (new_cap <= (size_t)d) new_cap <<= 1;
            Group **tmpg = (Group **)realloc(groups, new_cap * sizeof(Group *));
            groups = tmpg;
            for (size_t j = groups_cap; j < new_cap; ++j) groups[j] = NULL;
            groups_cap = new_cap;
        }

        /* lazily allocate the group structure for this divisor count */
        Group *g = groups[d];
        if (!g) {
            g = (Group *)malloc(sizeof(Group));
            g->len = 0;
            g->cap = 4;
            g->arr = (idx_t *)malloc(g->cap * sizeof(idx_t));
            groups[d] = g;
        }

        /* ensure the group has room to append */
        if (g->len == g->cap) {
            g->cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(g->arr, g->cap * sizeof(idx_t));
            g->arr = tmp;
        }

        g->arr[g->len++] = i;
    }

    max_n = up_to;
}

/* Return the a(n) value as defined by OEIS A301331. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    ensure_precompute(n);
    if (cached_flag[n]) return cache_arr[n];

    /* compute dp[0..n] for the current n */
    idx_t d = divcnt[n];
    Group *g = groups[d];          /* list of allowed part sizes */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;      /* fall back on out‑of‑memory, though this case won't happen */

    dp[0] = (seq_t)1;
    for (idx_t s = 1; s <= n; ++s) {
        seq_t sum = 0;
        for (size_t p = 0; p < g->len; ++p) {
            idx_t part = g->arr[p];
            if (part > s) break;
            sum += dp[s - part];
        }
        dp[s] = sum;
    }

    seq_t result = dp[n];
    free(dp);

    cache_arr[n] = result;
    cached_flag[n] = 1;
    return result;
}
