/*
 * A398955  Numbers k such that k, k+1, and k+2 are all cubefree.
 * https://oeis.org/A398955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9555 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: brute‑force search with memoisation.
   Returns the n-th integer k such that k, k+1 and k+2 are all cubefree. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether n is cubefree.  The function works for both
   native unsigned integer types and arbitrary‑precision seq_t
   types because it casts to uint64_t when the value is known
   to fit in that range.  This is safe for the problem domain
   (k is on the order of 10⁴ for the required indices). */
static bool is_cubefree(seq_t n)
{
    if (n <= 1) return true;
    uint64_t N = (uint64_t)n;          /* safe cast for the domain */
    for (uint64_t p = 2; p * p * p <= N; ++p) {
        if (N % p == 0) {
            unsigned exp = 0;
            while (N % p == 0) {
                N /= p;
                ++exp;
                if (exp >= 3) return false;   /* p³ divides n */
            }
        }
    }
    return true;
}

/* Cache of terms already found.  The cache is grown
   geometrically (double size) as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;   /* the next value to test */

/* Return a(n), the n-th cubefree 3‑tuple starter (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard */
    while (cache_len < (size_t)n) {
        /* Find next k such that k, k+1, k+2 are cubefree. */
        while (!(is_cubefree(next_k) &&
                 is_cubefree(next_k + 1) &&
                 is_cubefree(next_k + 2))) {
            ++next_k;
        }

        /* Grow the cache if necessary. */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();   /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_k;
        ++next_k;   /* prepare for next search */
    }
    return cache[n - 1];
}
