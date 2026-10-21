/*
 * A276397  Trajectory of 0 under the morphism 0 -> 001, 1 -> 0010.
 * https://oeis.org/A276397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2)*alpha) - floor((n+1)*alpha), where alpha =
 *            (sqrt(13)-3)/2 (the morphic Sturmian sequence 0→001, 1→0010)
 * Range:     u8 n <= 9587, u16 n <= 9587, u32 n <= 9587, u64 n <= 9587,
 *            u128 n <= 9587, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the generated 0/1 sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with the first symbol '0' */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* expand the cache until it contains at least n+1 terms */
    while (n >= cache_len) {
        idx_t cur_len = cache_len;

        /* the next length is at most 4 times the current length */
        idx_t new_cap = cur_len * 4;
        seq_t *next = (seq_t*)calloc(new_cap, sizeof(seq_t));
        idx_t pos = 0;

        for (idx_t i = 0; i < cur_len; ++i) {
            if (cache[i] == (seq_t)0) {
                /* 0 → 0,0,1 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
            } else {
                /* 1 → 0,0,1,0 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            }
        }

        free(cache);
        cache = next;
        cache_len = pos;  /* actual number of elements produced */
    }

    return cache[n];
}
