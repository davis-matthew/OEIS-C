/*
 * A375076  Numbers whose prime factorization exponents include at least one
 *          1, at least one 3 and no other exponents.
 * https://oeis.org/A375076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9540 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375076: Numbers whose prime factorisation exponents include
    at least one 1, at least one 3 and no other exponents.
    Sequence a(n) is generated in increasing order by testing every
    integer for the required exponent pattern.  The function is
    memoised so that calls can be made in any order.  This works
    for n up to ~10 000 in well under two seconds on typical
    64‑bit hardware. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Return true iff x has only exponent 1 or 3 in its prime factorisation
   and contains at least one of each.  Uses trial division. */
static bool is_candidate(uint64_t x) {
    if (x < 2) return false;
    bool has1 = false, has3 = false;

    /* check factor 2 */
    if (x % 2 == 0) {
        int exp = 0;
        while (x % 2 == 0) { x /= 2; ++exp; }
        if (exp != 1 && exp != 3) return false;
        if (exp == 1) has1 = true; else has3 = true;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) { x /= p; ++exp; }
            if (exp != 1 && exp != 3) return false;
            if (exp == 1) has1 = true; else has3 = true;
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (x > 1) {
        /* exp = 1, which is allowed */
        has1 = true;
    }

    return has1 && has3;
}

/* Helper to grow a seq_t array to accommodate at least `need` elements. */
#ifdef __cplusplus
template <class T>
static T *grow_seq(T *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
        while (new_cap < need) new_cap <<= 1;
        ptr = mem_resize(T, ptr, (new_cap * elem_size) / sizeof(T));
        *cap = new_cap;
    }
    return ptr;
}
#else
static void *grow_seq(void *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
        while (new_cap < need) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}
#endif

/* Global state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of terms stored */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Main function required by the build system */
seq_t a(idx_t n) {
    /* Grow cache to hold at least n terms */
    cache = (seq_t *)grow_seq(cache, &cache_cap, (size_t)n, sizeof(seq_t));

    while (cache_len < (size_t)n) {
        /* Test current candidate */
        if (is_candidate((uint64_t)next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
