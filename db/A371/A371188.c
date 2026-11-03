/*
 * A371188  Indices of the squarefree numbers in the sequence of cubefree
 *          numbers.
 * https://oeis.org/A371188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the index of the n‑th cubefree number that is also
 *            squarefree. The computation increments through the positive
 *            integers, counts those that are cubefree, and stores the counter
 *            value when the number is squarefree
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9681 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool is_cubefree_uint64(uint64_t x)
{
    if (x < 8u) return true;               /* 1–7 are always cubefree */
    for (uint64_t p = 2u; p * p * p <= x; ++p) {
        uint8_t cnt = 0;
        while (x % p == 0u) {
            x /= p;
            if (++cnt >= 3) return false;  /* exponent ≥ 3 found */
        }
    }
    return true; /* remaining factors have exponent ≤ 2 */
}

static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t p = 2u; p * p <= x; ++p) {
        if (x % p == 0u) {
            x /= p;
            if (x % p == 0u) return false; /* exponent ≥ 2 found */
        }
    }
    return true;
}

static seq_t *a_cache   = NULL;   /* dynamic array of sequence values   */
static size_t a_cache_len = 0;     /* number of elements currently stored */
static size_t a_cache_cap = 0;     /* capacity of the allocated array    */

static idx_t next_num      = 1;   /* next integer to test    */
static idx_t cubefree_idx  = 0;   /* current index in cubefree list */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, return 0 */

    /* Generate terms until we have at least n elements in a_cache. */
    while ((size_t)n > a_cache_len) {
        uint64_t m = (uint64_t)next_num++;    /* integer to test */

        if (is_cubefree_uint64(m)) {
            ++cubefree_idx;                   /* new cubefree number counted */

            if (is_squarefree_uint64(m)) {
                /* store the index of this cubefree, squarefree number */
                if (a_cache_len == a_cache_cap) {
                    size_t new_cap = (a_cache_cap == 0) ? 16 : (a_cache_cap << 1);
                    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
                    if (!tmp) exit(1);            /* fatal: out of memory   */
                    a_cache = tmp;
                    a_cache_cap = new_cap;
                }
                a_cache[a_cache_len++] = (seq_t)cubefree_idx;
            }
        }
    }
    return a_cache[n - 1];
}
