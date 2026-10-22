/*
 * A315339  Coordination sequence Gal.4.141.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8; a(0)=1, a(1)=6,
 *            a(2)=10, a(3)=16, a(4)=22, a(5)=28, a(6)=32, a(7)=38, a(8)=44
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t computed_until = 0;

    size_t idx = (size_t)n;

    /* Ensure cache can store at least the first nine terms and up to idx */
    size_t needed = idx + 1;
    if (needed < 9) needed = 9;
    if (needed > cache_len) {
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[needed];
#else
        new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
#endif
        for (size_t j = 0; j < cache_len; ++j) {
            new_cache[j] = cache[j];
        }
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_len = needed;
    }

    /* Initialise the nine initial terms (if not yet done) */
    if (computed_until < 9) {
        for (size_t i = computed_until; i < 9; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)1;  break;
                case 1: cache[i] = (seq_t)6;  break;
                case 2: cache[i] = (seq_t)10; break;
                case 3: cache[i] = (seq_t)16; break;
                case 4: cache[i] = (seq_t)22; break;
                case 5: cache[i] = (seq_t)28; break;
                case 6: cache[i] = (seq_t)32; break;
                case 7: cache[i] = (seq_t)38; break;
                case 8: cache[i] = (seq_t)44; break;
            }
        }
        computed_until = 9;
    }

    /* Compute the recurrence up to the requested index */
    if (idx + 1 > computed_until) {
        for (size_t i = computed_until; i <= idx; ++i) {
            cache[i] = cache[i - 1] + cache[i - 7] - cache[i - 8];
        }
        computed_until = idx + 1;
    }

    return cache[idx];
}
