/*
 * A307699  Numbers k such that there is no integer partition of k with
 *          exactly k-1 submultisets.
 * https://oeis.org/A307699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   _gcd_idx, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A307699 – Numbers k such that there is no integer partition of k with exactly k‑1 submultisets. */
/* Implementation uses a search over factorizations of k-1; the sequence terms are cached. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Include the helper that grows arrays of seq_t (works for both C and C++). */
#include "../utils/grow_seq_array.h"
#include "../utils/_gcd_idx.h"

/* Recursive search for a factorization of N = k - 1 into factors >= 2. */
static bool _dfs(idx_t k, idx_t rem, idx_t start, idx_t sum_m, idx_t gcd_m) {
    if (rem == 1) {
        /* All factors chosen: test the partition conditions. */
        return (k >= sum_m) && ((k - sum_m) % gcd_m == 0);
    }

    /* Try small factors first. */
    for (idx_t f = start; f * f <= rem; ++f) {
        if (rem % f == 0) {
            idx_t m = f - 1;
            idx_t new_sum = sum_m + m;
            if (new_sum > k) continue;
            idx_t new_gcd = _gcd_idx(gcd_m, m);
            if (_dfs(k, rem / f, f, new_sum, new_gcd)) return true;
        }
    }

    /* The remaining factor itself. */
    idx_t f = rem;
    if (f >= start) {
        idx_t m = f - 1;
        idx_t new_sum = sum_m + m;
        if (new_sum <= k) {
            idx_t new_gcd = _gcd_idx(gcd_m, m);
            if ((k >= new_sum) && ((k - new_sum) % new_gcd == 0)) return true;
        }
    }
    return false;
}

/* Return true iff k has a partition with exactly k-1 submultisets. */
static bool _has_valid_partition(idx_t k) {
    if (k <= 2) return false;            /* 0,1,2 never have such a partition */
    idx_t N = k - 1;
    if (N <= 1) return false;            /* k = 2 case handled above */
    return _dfs(k, N, 2, 0, 0);
}

/* Appends val to the dynamic array arr, growing if necessary. */
static void _arr_push(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        grow_seq_array(arr, cap, new_cap);
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Main sequence function requested by the build system. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;           /* next integer k to test (starts at 0) */

    while (cache_len < n) {
        if (! _has_valid_partition(next_k)) {
            _arr_push(&cache, &cache_len, &cache_cap, (seq_t)next_k);
        }
        ++next_k;
    }
    return cache[n - 1];
}
