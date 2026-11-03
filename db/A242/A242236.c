/*
 * A242236  Number of length n+5+1 0..5 arrays with every value 0..5 appearing
 *          at least once in every consecutive 5+2 elements, and new values
 *          0..5 introduced in order.
 * https://oeis.org/A242236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6) with initial
 *            values a(1)=21, a(2)=51, a(3)=106, a(4)=211, a(5)=416, a(6)=821
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behavior below offset is irrelevant */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of allocated indices (0 .. cache_size-1) */

    if (cache_size <= n) {
        idx_t needed = n + 1;        /* we need indices 0..n inclusive */
        cache = mem_resize(seq_t, cache, needed);
        if (!cache) { abort(); }      /* out of memory */

        /* fill values from old size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 6) {
                switch (i) {
                    case 1: cache[i] = (seq_t)21; break;
                    case 2: cache[i] = (seq_t)51; break;
                    case 3: cache[i] = (seq_t)106; break;
                    case 4: cache[i] = (seq_t)211; break;
                    case 5: cache[i] = (seq_t)416; break;
                    case 6: cache[i] = (seq_t)821; break;
                }
            } else {
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3] +
                           cache[i-4] + cache[i-5] + cache[i-6];
            }
        }
        cache_size = needed;
    }

    return cache[n];
}
