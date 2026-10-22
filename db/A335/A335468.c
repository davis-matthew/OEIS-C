/*
 * A335468  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (2,1,2).
 * https://oeis.org/A335468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for OEIS A335468.
 * The sequence consists of the indices k of compositions (in standard
 * graded reverse‑lexicographic order, A066099) whose part list contains a
 * subsequence (not necessarily contiguous) with the same relative order as
 * (2,1,2).  The implementation constructs each composition directly from
 * its index k, tests the pattern, and memoises the results.  It works
 * for all supported integer types (uint8_t … unsigned __int128).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

static bool matches_pattern(idx_t k)
{
    /* Determine the sum n: smallest n with 2^n-1 ≥ k.
       For k≥1, n = 64 - __builtin_clzll(k). */
    unsigned int n = 64 - __builtin_clzll(k);

    /* k lies among the compositions of n.
       Number of compositions of n-1 is 2^(n-1)-1. */
    idx_t base = ((idx_t)1 << (n - 1)) - 1;
    idx_t m1 = k - base - 1;            /* zero‑based index among the n‑part comps */

    /* Decode the composition parts.
       The reversed‑binary bits of m1 give separator flags.
       parts array size n (max 64). */
    unsigned parts[65];
    unsigned idx_part = 0;
    unsigned part = 1;
    for (unsigned i = 0; i < n - 1; ++i) {
        unsigned bit = (unsigned)((m1 >> i) & 1ULL);
        if (bit) {
            parts[idx_part++] = part;
            part = 1;
        } else {
            ++part;
        }
    }
    parts[idx_part++] = part;          /* last part */

    /* Precompute first and last occurrences of each part value. */
    unsigned first[65];
    unsigned last[65];
    for (unsigned v = 0; v <= n; ++v) {
        first[v] = UINT_MAX;
        last[v] = UINT_MAX;
    }
    for (unsigned i = 0; i < idx_part; ++i) {
        unsigned v = parts[i];
        if (first[v] == UINT_MAX) first[v] = i;
        last[v] = i;
    }

    /* Test for a subsequence (2,1,2): find a value v > middle part
       that appears both before and after the middle position. */
    for (unsigned j = 0; j < idx_part; ++j) {
        unsigned middle = parts[j];
        for (unsigned v = middle + 1; v <= n; ++v) {
            if (first[v] < j && last[v] > j) {
                return true;
            }
        }
    }
    return false;
}

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cacheV;
static idx_t next_k = 1;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* guard, though offset is 1 */

#ifdef __cplusplus
    while (cacheV.size() < n) {
        if (matches_pattern(next_k))
            cacheV.push_back((seq_t)next_k);
        ++next_k;
    }
    return cacheV[n - 1];
#else
    while (cache_len < n) {
        if (matches_pattern(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) exit(1);
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
#endif
}
