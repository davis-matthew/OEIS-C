/*
 * A345169  Numbers k such that the k-th composition in standard order is a
 *          non-alternating anti-run.
 * https://oeis.org/A345169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating the compositions in standard order until n
   valid indices are found.  A valid index k is one whose k‑th composition
   (graded reverse‑lexicographic, A066099) is an anti‑run (no equal adjacent
   parts) and is not alternating (does not strictly alternate up and down).  */

/* The function returns the n‑th such index.  It memoizes results so that
   subsequent calls are O(1).  Complexity per new term is O(log k). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Check whether the composition corresponding to k is an
   anti‑run and non‑alternating. */
static bool is_valid(idx_t k)
{
    /* At most 64 set bits in a 64‑bit integer. */
    uint8_t parts[64];
    int len = 0;
    idx_t word = k;
    idx_t idx = 0;
    idx_t prev = 0;

    /* Gather positions of 1‑bits (positions start at 1). */
    while (word) {
        ++idx;
        if (word & 1) {
            parts[len++] = (uint8_t)(idx - prev);
            prev = idx;
        }
        word >>= 1;
    }

    /* Reverse to obtain the composition. */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        uint8_t tmp = parts[i];
        parts[i] = parts[j];
        parts[j] = tmp;
    }

    /* Anti‑run: no equal adjacent parts. */
    for (int i = 0; i + 1 < len; ++i)
        if (parts[i] == parts[i + 1])
            return false;

    /* Non‑alternating check.  A composition is alternating if it satisfies
       either of the two strict patterns:
       pattern1: a0 < a1 > a2 < a3 > ...
       pattern2: a0 > a1 < a2 > a3 < ...
    */
    bool alt1 = true;
    bool alt2 = true;
    for (int i = 0; i + 1 < len; ++i) {
        if (i & 1) { /* odd index */
            if (parts[i] <= parts[i + 1]) alt1 = false;   /* should be > for pattern1 */
            if (parts[i] >= parts[i + 1]) alt2 = false;   /* should be < for pattern2 */
        } else {            /* even index */
            if (parts[i] >= parts[i + 1]) alt1 = false;   /* should be < for pattern1 */
            if (parts[i] <= parts[i + 1]) alt2 = false;   /* should be > for pattern2 */
        }
        if (!alt1 && !alt2)
            break;
    }
    return !(alt1 || alt2);  /* true if non‑alternating */
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: use std::vector for storage. */
    static std::vector<seq_t> cache;
    static idx_t last_k = 0;  /* last integer examined */

    while (cache.size() < (size_t)n) {
        ++last_k;
        if (is_valid(last_k))
            cache.push_back((seq_t)last_k);
    }
    return cache[n - 1];
#else
    /* C build: use realloc for storage. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;

    while (cache_len < (size_t)n) {
        ++last_k;
        if (is_valid(last_k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = new_cache;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
#endif
}
