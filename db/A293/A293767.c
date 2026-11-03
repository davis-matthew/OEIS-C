/*
 * A293767  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - 1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) - 1, a(0) = 1, a(1) = 3, b(0) =
 *            2, b(1) = 4, where b(n) is the complementary (missing) positive
 *            integer not occurring in the sequence a
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* binary search: upper_bound_seq(arr[0..n-1], val) */
static idx_t upper_bound_seq(const seq_t *arr, idx_t n, seq_t val) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (arr[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* k-th missing positive integer (0‑based) from the sorted array arr[0..cnt-1] */
static seq_t kth_missing(const seq_t *arr, idx_t cnt, idx_t k) {
    seq_t low  = 1;
    /* high must be sufficiently large; arr[cnt-1] + (k+1) is always enough */
    seq_t high = arr[cnt-1] + (k + 1);

    while (low < high) {
        seq_t mid = (low + high) >> 1;
        idx_t m   = upper_bound_seq(arr, cnt, mid); /* how many a's ≤ mid */
        seq_t missing = mid - (seq_t)m;             /* numbers ≤ mid missing from a */

        if (missing > (seq_t)k)
            high = mid;
        else
            low = mid + 1;
    }
    return low; /* k-th missing number (0‑based) */
}

static seq_t *a_cache   = NULL;   /* memoised a[0..] values   */
static size_t a_cap     = 0;       /* allocated array capacity */
static idx_t  a_len     = 0;       /* last computed index (≥0) */

/* Ensure that a_cache contains a[0..n] */
static void ensure_cache(idx_t n) {
    /* initialise base values if cache empty */
    if (a_cache == NULL) {
        a_cap = 2;
        a_cache = mem_new(seq_t, a_cap);
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_len = 1; /* last computed index is 1 */
    }
    if (n <= a_len) return;

    /* enlarge storage if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* compute missing terms and a[n] iteratively */
    for (idx_t new_n = a_len + 1; new_n <= n; ++new_n) {
        idx_t k     = new_n - 1;                      /* index for b(n‑1)   */
        seq_t b_val = kth_missing(a_cache, new_n, k); /* missing number   */
        seq_t val   = a_cache[new_n - 1] + a_cache[new_n - 2] + b_val - 1;
        a_cache[new_n] = val;
    }
    a_len = n;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return a_cache[n];
}
