/*
 * A330082  a(n) = 5*A064038(n+1).
 * https://oeis.org/A330082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            3*a(n-1)-6*a(n-2)+10*a(n-3)-12*a(n-4)+12*a(n-5)-10*a(n-6)+6*a(n-7)-3*a(n-8)+a(n-9)
 *            with initial values a(0)=0, a(1)=5, a(2)=15, a(3)=15, a(4)=25,
 *            a(5)=75, a(6)=105, a(7)=70, a(8)=90
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init9[9] = {0,5,15,15,25,75,105,70,90};
static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_len = 0;     /* number of terms stored in cache */

seq_t a(idx_t n)
{
    /* small n: return from the known seed values */
    if (n < 9)
        return init9[(size_t)n];

    /* first time this function is called: allocate cache and store the seed values */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init9[i];
    }

    /* extend the cache if needed */
    size_t target = (size_t)n;
    if (target >= cache_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t pos = (seq_t)3 * cache[i-1]
                      + (seq_t)10 * cache[i-3]
                      + (seq_t)12 * cache[i-5]
                      + (seq_t)6  * cache[i-7]
                      +          cache[i-9];
            seq_t neg = (seq_t)6  * cache[i-2]
                      + (seq_t)12 * cache[i-4]
                      + (seq_t)10 * cache[i-6]
                      + (seq_t)3  * cache[i-8];
            cache[i] = pos - neg;
        }
        cache_len = new_len;
    }

    return cache[target];
}
