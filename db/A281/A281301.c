/*
 * A281301  Trajectory of 1000004999700144385 under the "Reverse and Add!"
 *          operation.
 * https://oeis.org/A281301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1000004999700144385; a(n+1)=a(n)+rev(a(n))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 54, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse.h"

const seq_t START = 1000004999700144385ULL;

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;  /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;  /* out of memory, unlikely in tests */
        cache[0] = START;
        cache_len = 1;
    }

    /* grow cache to at least n+1 entries */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1];
        seq_t rev  = reverse(prev);
        seq_t next = prev + rev;

        /* ensure capacity */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (tmp == NULL) return 0;  /* out of memory */
            cache = tmp;
        }
        cache[cache_len] = next;
        cache_len += 1;
    }

    return cache[n];
}
