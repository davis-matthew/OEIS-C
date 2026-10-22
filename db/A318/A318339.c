/*
 * A318339  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + a(n-3) - 5*a(n-4) - 2*a(n-5) +
 *            2*a(n-6) + a(n-7) - a(n-8) for n > 9 Initial values: a(1)=8,
 *            a(2)=20, a(3)=36, a(4)=67, a(5)=148, a(6)=343, a(7)=817,
 *            a(8)=1975, a(9)=4788
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n) {
    if (n == 0)   /* offset starts at 1, but guard against 0 */
        return (seq_t)0;

    /* allocate cache if necessary */
    if (!cache || n > cache_max) {
        /* enlarge the cache if required */
        if (!cache || cache_max < n) {
            seq_t *new_cache = alloc_seq_array((size_t)n + 1);
            if (cache) {
                for (idx_t i = 1; i <= cache_max; ++i)
                    new_cache[i] = cache[i];
                free_seq(cache);
            }
            cache      = new_cache;
            cache_max  = 0;          /* will be updated after filling */
        }
        /* compute values from cache_max+1 up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* initial values for i ≤ 9 */
            if (i <= 9) {
                switch (i) {
                    case 1: cache[i] = (seq_t)8;   break;
                    case 2: cache[i] = (seq_t)20;  break;
                    case 3: cache[i] = (seq_t)36;  break;
                    case 4: cache[i] = (seq_t)67;  break;
                    case 5: cache[i] = (seq_t)148; break;
                    case 6: cache[i] = (seq_t)343; break;
                    case 7: cache[i] = (seq_t)817; break;
                    case 8: cache[i] = (seq_t)1975;break;
                    case 9: cache[i] = (seq_t)4788;break;
                }
            } else {
                /* compute using the recurrence without intermediate negatives */
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                seq_t a6 = cache[i-6];
                seq_t a7 = cache[i-7];
                seq_t a8 = cache[i-8];
                seq_t p = a1 + (seq_t)4 * a2 + a3 + (seq_t)2 * a6 + a7;
                seq_t q = (seq_t)5 * a4 + (seq_t)2 * a5 + a8;
                cache[i] = p - q;
            }
        }
        cache_max = n;
    }
    return cache[n];
}
