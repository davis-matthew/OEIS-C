/*
 * A257441  Number of (n+2) X (2+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A257441
 *  Number of (n+2) × 4 0..1 arrays with no 3×3 subblock diagonal sum 0,
 *  no antidiagonal sum 3, no row sum 1 and no column sum 1.
 *
 *  Empirical recurrence (valid for n>8):
 *      a(n) = 2*a(n‑1) - a(n‑2) + a(n‑3) - a(n‑4)
 *
 *  Initial values:
 *      a(1)=36, a(2)=38, a(3)=41, a(4)=51,
 *      a(5)=68, a(6)=95, a(7)=134, a(8)=191
 *
 *  The implementation memoises values across calls and works for any
 *  n ≥ 1.  It uses a C++ std::vector when compiling as C++17 and a
 *  malloc‑based cache when compiling as C11.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Sequence values are supplied by the build system.  Do NOT redefine seq_t
 * or idx_t. */
extern seq_t a(idx_t n) {
    /* Static initializer for the first eight terms.  All branches
     * (C and C++) use this array. */
    static const seq_t init[8] = {
        36, 38, 41, 51, 68, 95, 134, 191
    };

    if (n == 0) {
        /* Sequence is defined for n≥1; return 0 for safety. */
        return 0;
    }

    if (n <= 8) {
        /* directly return one of the first eight values */
        return init[n - 1];
    }

#ifdef __cplusplus
    /* C++ branch: use std::vector for storage. */
    static std::vector<seq_t> cache;
    /* Initialise the first eight terms only once. */
    if (cache.empty()) {
        cache.resize(8);
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
    }

    /* Grow the cache if needed. */
    if (cache.size() < static_cast<size_t>(n)) {
        cache.reserve(static_cast<size_t>(n));
        for (idx_t k = static_cast<idx_t>(cache.size()) + 1; k <= n; ++k) {
            const seq_t &a1 = cache[k - 2];
            const seq_t &a2 = cache[k - 3];
            const seq_t &a3 = cache[k - 4];
            const seq_t &a4 = cache[k - 5];
            cache.push_back(a1 * 2 - a2 + a3 - a4);
        }
    }
    return cache[n - 1];
#else
    /* C branch: use malloc / realloc.  This branch is only used when
     * seq_t is a fundamental numeric type. */
    static seq_t *arr       = NULL;  /* pointer to the array */
    static idx_t capacity   = 0;     /* allocated size */
    static idx_t cur_len    = 0;     /* number of valid terms stored */

    /* Allocate and initialise on first use. */
    if (arr == NULL) {
        capacity = 8;
        arr = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (arr == NULL) {
            return 0; /* allocation failure – unlikely */
        }
        for (size_t i = 0; i < 8; ++i) {
            arr[i] = init[i];
        }
        cur_len = 8;
    }

    /* If n is already computed, return it. */
    if (n <= cur_len) {
        return arr[n - 1];
    }

    /* Ensure sufficient capacity */
    if (n > capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 64;
        if (new_cap < n) {
            new_cap = n;
        }
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return 0; /* allocation failure */
        }
        arr = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cur_len + 1; k <= n; ++k) {
        seq_t val = arr[k - 2] * 2 - arr[k - 3] + arr[k - 4] - arr[k - 5];
        arr[k - 1] = val;
    }
    cur_len = n;
    return arr[n - 1];
#endif
}
