/*
 * A310640  Coordination sequence Gal.6.527.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-7) - a(n-8) for n > 9;
   Initial values: a(0)=1, a(1)=4, a(2)=6, a(3)=14, a(4)=17,
   a(5)=21, a(6)=28, a(7)=28, a(8)=36, a(9)=36 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

#if defined(__cplusplus)
#define ALLOC_SEQ(cnt) reinterpret_cast<seq_t*>(malloc(sizeof(seq_t)*(cnt)))
#define REALLOC_SEQ(ptr,cnt) reinterpret_cast<seq_t*>(realloc(ptr, sizeof(seq_t)*(cnt)))
#else
#define ALLOC_SEQ(cnt) (seq_t*)malloc(sizeof(seq_t)*(cnt))
#define REALLOC_SEQ(ptr,cnt) (seq_t*)realloc(ptr, sizeof(seq_t)*(cnt))
#endif

seq_t a(idx_t n) {
    static const seq_t base[10] = {
        1, 4, 6, 14, 17, 21, 28, 28, 36, 36
    };

    if (n < 10)
        return base[(size_t)n];

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (!cache) {
            cache = ALLOC_SEQ(new_len);
            if (!cache)
                abort();
        } else {
            seq_t *tmp = REALLOC_SEQ(cache, new_len);
            if (!tmp)
                abort();
            cache = tmp;
        }

        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 10) {
                cache[i] = base[(size_t)i];
            } else {
                cache[i] = cache[i-1] + cache[i-7] - cache[i-8];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
