/*
 * A384908  Start with a list L of positive integers. At each step n, let the
 *          center be the smallest number that has not been used as a center
 *          before with index m > 1. For all i < m, swap L(i) with L(i+m).
 *          a(n) = L(1).
 * https://oeis.org/A384908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute OEIS sequence A384908 by simulating the
   “center‑swap” process described in the OEIS entry. It works in
   both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
/* -------------------- C++ implementation -------------------- */
#include <vector>

static std::vector<seq_t> L(1);                 /* L[0] unused, indices 1.. */
static std::vector<size_t> idx_of_value(1);    /* idx_of_value[v] = pos of v in L */
static std::vector<char> used_center(1, 0);     /* used_center[v] == 1 if v was a center */
static std::vector<seq_t> a_cache(1);          /* a_cache[n] = a(n) */

static void ensure_list_cap(size_t cap)
{
    size_t sz = L.size() - 1;   /* current maximum index */
    if (cap <= sz) return;
    L.resize(cap + 1);
    idx_of_value.resize(cap + 1);
    used_center.resize(cap + 1, 0);
    for (size_t i = sz + 1; i <= cap; ++i) {
        L[i] = (seq_t)i;
        idx_of_value[i] = i;
        used_center[i] = 0;
    }
}

static void ensure_cache_cap(size_t cap)
{
    if (cap < a_cache.size() - 1) return;
    a_cache.resize(cap + 1);
}

/* -------------------- end C++ implementation -------------------- */
#else
/* -------------------- C implementation -------------------- */
static seq_t *L = NULL;                  /* 1‑based array of the current list */
static size_t *idx_of_value = NULL;      /* current index of each value */
static bool *used_center = NULL;         /* whether a value has been a center */
static seq_t *a_cache = NULL;            /* a(n) values */

static size_t L_len = 0;                  /* maximum index currently allocated */
static size_t cache_len = 0;              /* size of a_cache array */

static void ensure_list_cap(size_t cap)
{
    if (cap <= L_len) return;
    size_t new_cap = (L_len > 0) ? L_len : 64;
    while (new_cap < cap) new_cap = new_cap * 2 + 1;

    seq_t *new_L = (seq_t*)realloc(L, (new_cap + 1) * sizeof(seq_t));
    size_t *new_idx = (size_t*)realloc(idx_of_value, (new_cap + 1) * sizeof(size_t));
    bool *new_used = (bool*)realloc(used_center, (new_cap + 1) * sizeof(bool));
    if (!new_L || !new_idx || !new_used) exit(1);

    for (size_t i = L_len + 1; i <= new_cap; ++i) {
        new_L[i] = (seq_t)i;
        new_idx[i] = i;
        new_used[i] = false;
    }

    L = new_L; idx_of_value = new_idx; used_center = new_used;
    L_len = new_cap;
}

static void ensure_cache_cap(size_t cap)
{
    if (cap < cache_len) return;
    size_t new_cap = (cache_len > 0) ? cache_len : 64;
    while (new_cap < cap) new_cap = new_cap * 2 + 1;

    seq_t *new_cache = (seq_t*)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);

    a_cache = new_cache;
    cache_len = new_cap;
}

/* -------------------- end C implementation -------------------- */
#endif

/* -------------------- main function -------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 for safety */

    /* initialise static state on first call */
    static idx_t current_step = 0;
    if (current_step == 0) {
#ifdef __cplusplus
        ensure_list_cap(1);   /* L[1] = 1, idx_of_value[1] = 1, used_center[1] = 0 */
        ensure_cache_cap(1);
#else
        ensure_list_cap(1);
        ensure_cache_cap(1);
#endif
    }

    size_t target = (size_t)n;   /* n >= 1 and fits into size_t for all practical n */

    /* grow cache to hold the requested term */
    ensure_cache_cap(target);

    /* compute missing steps */
    while ((idx_t)current_step < target) {
        ++current_step;

        /* find smallest value that has not been a center and whose index > 1 */
        size_t center = 0;
        for (size_t v = 1; ; ++v) {
#ifdef __cplusplus
            if (v > L.size() - 1) ensure_list_cap(v);
            if (!used_center[v] && idx_of_value[v] > 1) {
                center = v;
                break;
            }
#else
            if (v > L_len) ensure_list_cap(v);
            if (!used_center[v] && idx_of_value[v] > 1) {
                center = v;
                break;
            }
#endif
        }

        size_t m = idx_of_value[center];

        /* mark as used and ensure list large enough for swaps */
#ifdef __cplusplus
        used_center[center] = 1;
        if (2 * m - 1 > L.size() - 1) ensure_list_cap(2 * m - 1);
#else
        used_center[center] = true;
        if (2 * m - 1 > L_len) ensure_list_cap(2 * m - 1);
#endif

        /* perform the swaps */
        for (size_t i = 1; i < m; ++i) {
            size_t j = i + m;
#ifdef __cplusplus
            seq_t v1 = L[i], v2 = L[j];
            L[i] = v2; L[j] = v1;
            idx_of_value[(size_t)v1] = j;
            idx_of_value[(size_t)v2] = i;
#else
            seq_t v1 = L[i], v2 = L[j];
            L[i] = v2; L[j] = v1;
            idx_of_value[(size_t)v1] = j;
            idx_of_value[(size_t)v2] = i;
#endif
        }

        /* store the first element */
        a_cache[current_step] = L[1];
    }

    return a_cache[target];
}
