/*
 * A322424  Start with (1, 2, 3, 4); when the sequence has n terms, extend it
 *          by appending a(n) copies of a(1..n-1). (Cf. "Look to the left"
 *          sequence A293630.)
 * https://oeis.org/A322424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: start with (1, 2, 3, 4); when the sequence has n terms,
   extend it by appending a(n) copies of a(1..n-1). */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t  cache_len = 0;  /* number of elements stored in cache */

/* Ensure the cache has at least "needed" slots.  Reallocates as
   necessary, copying existing elements.  Works for arbitrary-precision
   types because the copy is element‑by‑element. */
static void cache_ensure(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    seq_free_array(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Initialize the cache with the first four values. */
static void cache_init(void)
{
    if (cache_len == 0) {
        cache_ensure(4);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 4;
        cache_len = 4;
    }
}

/* Return the n-th element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n <= 0)          /* out of range – may not occur */
        return 0;
    cache_init();

    while (cache_len < n) {
        idx_t cur_len   = cache_len;          /* current length of seq */
        seq_t copies    = cache[cur_len - 1];  /* a(cur_len) */
        size_t seg_len  = cur_len - 1;         /* length of the segment to copy */
        size_t total_new = (size_t)copies * seg_len;

        cache_ensure(cache_len + total_new);

        for (seq_t t = 0; t < copies; ++t) {
            size_t dest = cache_len + (size_t)t * seg_len;
            for (size_t j = 0; j < seg_len; ++j) {
                cache[dest + j] = cache[j];
            }
        }
        cache_len += total_new;
    }

    return cache[n - 1];
}
