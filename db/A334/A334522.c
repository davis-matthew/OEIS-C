/*
 * A334522  Numbers that are either congruent to +-4 mod 9, or else are cubes
 *          or twice cubes.
 * https://oeis.org/A334522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are congruent to ±4 mod 9, or cubes, or twice cubes */
/* The function generates the sequence on demand and memoises the values. */
/* All arithmetic is performed on seq_t, letting the build pick the integer type. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"

#ifdef __cplusplus
#include <vector>
/* In C++ the cache is a std::vector, which handles construction/destruction of seq_t. */
static std::vector<seq_t> cache;
static seq_t next_candidate = 1;
#else
/* In plain C we use a manually grown array. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */
static seq_t next_candidate = 1;

static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* Test whether a number satisfies the definition of the sequence. */
static bool is_candidate(seq_t x)
{
    /* Congruent to ±4 mod 9 (i.e. 4 or 5). */
    seq_t r9 = x % 9;
    if (r9 == 4 || r9 == 5)
        return true;

    /* Perfect cube. */
    seq_t c = floor_cube_root(x);
    if (c * c * c == x)
        return true;

    /* Twice a perfect cube. */
    if ((x & 1) == 0) {          /* x is even */
        seq_t half = x / 2;
        seq_t d = floor_cube_root(half);
        if (d * d * d == half)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0; return 0 for safety */

#ifdef __cplusplus
    /* Grow the vector until it contains n terms. */
    while (cache.size() < (size_t)n) {
        if (is_candidate(next_candidate)) {
            cache.push_back(next_candidate);
        }
        ++next_candidate;
    }
    return cache[n - 1];
#else
    /* Grow the dynamic array until it contains n terms. */
    while (cache_len < (size_t)n) {
        if (is_candidate(next_candidate)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
#endif
}
