/*
 * A277150  Lexicographically least sequence of nonnegative integers that
 *          avoids 7/6-powers.
 * https://oeis.org/A277150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 136 terms through n = 9909 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the lexicographically least sequence of non‑negative integers
   that avoids 7/6‑powers.  The sequence is built greedily: at each
   position we append the smallest integer that does not create a factor
   of the form S followed by the first |S|/6 symbols of S (the definition
   of a 7/6–power).  The array is memoised between calls. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether the sequence of length L (seq_cache[0..L-1]) contains
   a 7/6‑power.  Returns true if the sequence is free of such powers. */
static bool is_free_7over6(const seq_t *s, size_t L)
{
    for (size_t k = 1; 7 * k <= L; ++k) {
        size_t start = L - 7 * k;
        size_t mid   = start + 6 * k;
        bool ok = true;
        for (size_t j = 0; j < k; ++j) {
            if (s[start + j] != s[mid + j]) { ok = false; break; }
        }
        if (ok) return false;   /* forbidden pattern found */
    }
    return true;
}

/* static storage for the memoised sequence */
static seq_t *seq_cache = NULL;     /* allocated storage */
static size_t   cache_cap = 0;      /* capacity of seq_cache */
static size_t   cache_sz  = 0;      /* number of valid elements in seq_cache */

/* Ensure that seq_cache can hold at least n+1 elements.  */
static void grow_cache(size_t n_needed)
{
    if (n_needed + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n_needed + 1) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp) abort();  /* allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Public function: return a(n) for the sequence.  */
seq_t a(idx_t n)
{
    /* build the sequence up to index n on demand */
    while (cache_sz <= (size_t)n) {
        grow_cache(cache_sz);
        seq_t cand = 0;
        while (true) {
            seq_cache[cache_sz] = cand;
            if (is_free_7over6(seq_cache, cache_sz + 1))
                break;               /* acceptable candidate */
            ++cand;                 /* try next integer */
        }
        ++cache_sz;
    }
    return seq_cache[(size_t)n];
}
