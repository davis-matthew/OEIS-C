/*
 * A334041  (a(n-2) XOR a(n-1)) OR (highest bit of a(n-2))*2 OR 1; a(0)=2,
 *          a(1)=3.
 * https://oeis.org/A334041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=2, a(1)=3 a(n) = (a(n-2) XOR a(n-1)) OR (1 <<
 *            bitlen(a(n-2))) OR 1
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 42 (2026-11-03)
 * Helpers:   bit_length, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static size_t  sz    = 0;   /* number of computed elements */

    /* initialise cache with the two base values */
    if (cap == 0) {
        cap = 2;
        cache = mem_new(seq_t, cap);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        sz = 2;
    }

    if (n < 2)
        return cache[(size_t)n];

    /* grow capacity if needed */
    if (cap <= (size_t)n) {
        size_t newcap = cap > 0 ? cap << 1 : 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        grow_seq_array(&cache, &cap, newcap);
    }

    /* compute all missing terms up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t prev2 = cache[i-2];
        seq_t prev1 = cache[i-1];
        seq_t xored = prev2 ^ prev1;
        seq_t high  = ((seq_t)1) << bit_length(prev2);
        cache[i] = xored | high | ((seq_t)1);
    }
    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
