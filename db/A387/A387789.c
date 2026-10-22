/*
 * A387789  a(1) = 0, a(2*n) = a(n)+1, a(2*n+1) = floor((a(n)+a(n+1))/2) for n
 *          >= 1.
 * https://oeis.org/A387789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9194, u16 n <= 9194, u32 n <= 9194, u64 n <= 9194,
 *            u128 n <= 9194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 0;
   a(2*n)   = a(n) + 1;
   a(2*n+1) = floor((a(n)+a(n+1))/2)
*/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_size = 2; /* indices 0..1 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        computed = (bool *)malloc(cache_size * sizeof(bool));
        memset(cache, 0, cache_size * sizeof(seq_t));
        memset(computed, 0, cache_size * sizeof(bool));
        cache[1] = 0;
        computed[1] = true;
    }
    while (n + 1 > cache_size) {
        idx_t new_cap = cache_size * 2;
        if (new_cap <= n) new_cap = n * 2 + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        computed = (bool *)realloc(computed, new_cap * sizeof(bool));
        memset(computed + cache_size, 0,
               (new_cap - cache_size) * sizeof(bool));
        cache_size = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */
    ensure_capacity(n);
    if (computed[n]) return cache[n];

    seq_t val;
    if (n & 1) {                    /* odd: 2*k+1 */
        idx_t k = n >> 1;
        val = (a(k) + a(k + 1)) / 2;
    } else {                        /* even: 2*k */
        idx_t k = n >> 1;
        val = a(k) + 1;
    }
    cache[n] = val;
    computed[n] = true;
    return val;
}
