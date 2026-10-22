/*
 * A316513  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) for n > 8
 *            Initial values: a(1)=4, a(2)=21, a(3)=28, a(4)=75, a(5)=174,
 *            a(6)=414, a(7)=1002, a(8)=2398
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) continue;              /* unused index 0 */
            if (i <= 8) {
                switch (i) {
                    case 1: cache[i] = (seq_t)4;    break;
                    case 2: cache[i] = (seq_t)21;   break;
                    case 3: cache[i] = (seq_t)28;   break;
                    case 4: cache[i] = (seq_t)75;   break;
                    case 5: cache[i] = (seq_t)174;  break;
                    case 6: cache[i] = (seq_t)414;  break;
                    case 7: cache[i] = (seq_t)1002; break;
                    case 8: cache[i] = (seq_t)2398; break;
                }
            } else {
                cache[i] =  cache[i-1]
                           + (seq_t)2 * cache[i-2]
                           + (seq_t)3 * cache[i-3]
                           + cache[i-4]
                           - cache[i-5];
            }
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* value outside the requested range */
    ensure_cache(n);
    return cache[n];
}
