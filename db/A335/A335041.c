/*
 * A335041  Numbers whose binary representation encodes a (binary) min-heap on
 *          elements from the set {0,1} with root at the least significant
 *          bit.
 * https://oeis.org/A335041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1940, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335041: numbers whose binary representation encodes a (binary) min‑heap
   on elements {0,1} with the root at the least‑significant bit.
   For a binary number x, let LSB be index 0, next bit index 1, etc.
   The sequence contains all non‑negative integers x such that
   whenever bit i is 1, all existing child bits (indices 2*i+1,
   2*i+2) are also 1.  This property is tested directly in heap_valid.
   The function builds the sequence lazily and memoises results.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"   /* bitlen_seq_t(seq_t) */

static bool heap_valid(seq_t x)
{
    idx_t len = bitlen_seq_t(x);
    for (idx_t i = 0; i < len; ++i) {
        if ((x >> i) & 1) {                 /* bit i is 1 */
            idx_t c1 = 2 * i + 1;
            idx_t c2 = 2 * i + 2;
            if (c1 < len && ((x >> c1) & 1) == 0) return false;
            if (c2 < len && ((x >> c2) & 1) == 0) return false;
        }
    }
    return true;
}

/* cache for computed terms; indexes 1..cache_len-1 stored in cache[0..]
   using 1‑based sequence indices for convenience */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently cached */
static size_t cache_cap = 0;   /* allocated capacity of the cache array */

static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but safe return */

    if (n > cache_len) {
        ensure_cache((size_t)n);
        seq_t candidate = (cache_len > 0) ? (cache[cache_len - 1] + 1) : (seq_t)0;
        size_t count = cache_len;

        while (count < (size_t)n) {
            if (heap_valid(candidate)) {
                cache[count] = candidate;
                ++count;
            }
            ++candidate;
        }
        cache_len = count;
    }

    return cache[n - 1];
}
