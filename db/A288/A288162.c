/*
 * A288162  Numbers whose prime factors are 2 and 13.
 * https://oeis.org/A288162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*A107326(n)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 119, u64 n <= 513,
 *            u128 n <= 2132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107326(n) is the nth positive integer whose prime factors are only 2 and 13. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache for the A107326 sequence */
    static seq_t *cache = NULL;   /* array of A107326 values   */
    static size_t cache_cap = 0;  /* capacity of the array     */
    static size_t cache_len = 0;  /* number of elements computed */
    static size_t i2 = 0;         /* next index for *2         */
    static size_t i13 = 0;        /* next index for *13        */

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure */
    }

    /* grow and generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;     /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (cache_len == 0) {
            /* first element A107326(1) = 1 */
            cache[0] = (seq_t)1;
            cache_len = 1;
            continue;
        }

        seq_t cand2  = cache[i2] * (seq_t)2;
        seq_t cand13 = cache[i13] * (seq_t)13;

        if (cand2 <= cand13) {
            cache[cache_len] = cand2;
            i2++;
            if (cand2 == cand13) i13++;   /* avoid duplicate  */
        } else {
            cache[cache_len] = cand13;
            i13++;
        }
        cache_len++;
    }

    /* return the requested term multiplied by 26 */
    return cache[n - 1] * (seq_t)26;
}
