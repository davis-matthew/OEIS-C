/*
 * A281980  Numbers of the form x^4 + y^2 with x^2 + 24*y a square, where x
 *          and y are nonnegative integers.
 * https://oeis.org/A281980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 227, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281980: Numbers of the form x^4 + y^2 with x^2 + 24*y a square
 * (offset 1).  The function enumerates all (x,y) with m² = x² + 24y
 * and keeps the values x^4 + y^2 sorted.  Calls are memoised but not
 * required in order. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* integer floor of fourth root (t^4 ≤ n < (t+1)^4) */
static uint64_t floor_pow4(uint64_t n)
{
    uint64_t lo = 0, hi = 1;
    /* double hi until hi^4 > n or overflow */
    while (1) {
        __int128 p = (__int128)hi * hi * hi * hi;
        if (p > n) break;
        if (hi == UINT64_MAX >> 1) break; /* avoid overflow */
        hi <<= 1;
    }
    /* binary search between lo and hi */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 p = (__int128)mid * mid * mid * mid;
        if (p <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* comparison function for qsort (used only in the C build) */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* C build helpers                                                      */
/* --------------------------------------------------------------------- */
#ifndef __cplusplus

/* generate all values ≤ limit and store them in the caller supplied
   array (sorted, unique). The caller receives the pointer to the
   allocated array in *out and its length in *len. */
static void generate_up_to(uint64_t limit, seq_t **out, size_t *len)
{
    uint64_t xmax = floor_pow4(limit);
    /* 576 ≈ 24 * 24, a safe over‑estimate for the m‑range */
    uint64_t mmax = floor_pow4(576ULL * limit);

    size_t cap = 64;
    size_t cnt = 0;
    seq_t *cand = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cand) exit(1);

    for (uint64_t x = 0; x <= xmax; ++x) {
        __int128 x2 = (__int128)x * x;
        for (uint64_t m = x; m <= mmax; ++m) {
            __int128 m2 = (__int128)m * m;
            __int128 diff = m2 - x2;
            if (diff % 24 != 0) continue;
            __int128 y = diff / 24;
            __int128 val128 = (__int128)x*x*x*x + (__int128)y*y;
            if (val128 > limit) continue;
            seq_t v = (seq_t)val128;
            if (cnt == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cand, cap * sizeof(seq_t));
                if (!tmp) { free(cand); exit(1); }
                cand = tmp;
            }
            cand[cnt++] = v;
        }
    }

    /* sort and deduplicate */
    qsort(cand, cnt, sizeof(seq_t), seq_cmp);
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq == 0 || cand[i] != cand[uniq - 1])
            cand[uniq++] = cand[i];
    }
    *out = cand;
    *len = uniq;
}

#endif /* !__cplusplus */

/* --------------------------------------------------------------------- */
/* C++ build helpers                                                   */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
#include <vector>
#include <algorithm>

/* generate all values ≤ limit; returns a sorted vector with unique entries */
static std::vector<seq_t> generate_up_to_cpp(uint64_t limit)
{
    std::vector<seq_t> cand;
    uint64_t xmax = floor_pow4(limit);
    uint64_t mmax = floor_pow4(576ULL * limit);

    for (uint64_t x = 0; x <= xmax; ++x) {
        __int128 x2 = (__int128)x * x;
        for (uint64_t m = x; m <= mmax; ++m) {
            __int128 m2 = (__int128)m * m;
            __int128 diff = m2 - x2;
            if (diff % 24 != 0) continue;
            __int128 y = diff / 24;
            __int128 val128 = (__int128)x * x * x * x + (__int128)y * y;
            if (val128 > limit) continue;
            cand.push_back((seq_t)val128);
        }
    }

    std::sort(cand.begin(), cand.end());
    cand.erase(std::unique(cand.begin(), cand.end()), cand.end());
    return cand;
}
#endif /* __cplusplus */

/* --------------------------------------------------------------------- */
/* public function                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, but keeps type safety */

#ifdef __cplusplus
    /* static cache */
    static std::vector<seq_t> cache;
    static uint64_t limit = 1;

    while (cache.size() < (size_t)n) {
        std::vector<seq_t> cand = generate_up_to_cpp(limit);
        if (cand.empty()) { /* safety */
            limit = limit * 2 + 1;
            continue;
        }
        if (cand.size() < n) { /* not enough terms yet */
            limit = limit * 2 + 1;
            continue;
        }
        cache.swap(cand); /* keep the sorted, unique list */
    }
    return cache[(size_t)n - 1];

#else
    /* static cache */
    static seq_t *cached_vals = NULL;
    static size_t cached_len = 0;
    static uint64_t limit = 1;

    while (cached_len < (size_t)n) {
        free(cached_vals);
        cached_vals = NULL;
        cached_len = 0;

        generate_up_to(limit, &cached_vals, &cached_len);
        if (cached_len >= (size_t)n) break;

        limit = limit * 2 + 1; /* enlarge and retry */
    }
    return cached_vals[(size_t)n - 1];
#endif
}
