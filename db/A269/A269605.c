/*
 * A269605  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) – 353*a(n-2) + 1601*a(n-3) – 435*a(n-4) –
 *            14505*a(n-5) + 7118*a(n-6) + 65542*a(n-7) + 66279*a(n-8) +
 *            19971*a(n-9) Initial values: a(1)=8, a(2)=64, a(3)=504,
 *            a(4)=3962, a(5)=31024, a(6)=242226, a(7)=1886252, a(8)=14654952,
 *            a(9)=113629480. The implementation uses a dynamic programm...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static bool initialized = false;
    static seq_t *cache = NULL;        /* cache of a(1) … a(k) */
    static idx_t cache_size = 0;       /* current cached length */

    /* dp_prev[last][prev_rep]  last = 0..7,  prev_rep = 0..7 or 8 (none)   */
    static seq_t dp_prev[8][9];
    static seq_t dp_next[8][9];

    if (!initialized) {
        /* base case n = 1  (no repeat yet) */
        memset(dp_prev, 0, sizeof(dp_prev));
        for (int v = 0; v < 8; ++v) {
            dp_prev[v][8] = 1;            /* last = v, prev_rep = none */
        }
        cache_size = 1;
        cache      = seq_alloc_new_array(1);
        cache[0]   = 8;                   /* total for n = 1 */
        initialized = true;
    }

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* extend computation up to length n */
    for (idx_t len = cache_size + 1; len <= n; ++len) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int last = 0; last < 8; ++last) {
            for (int prev = 0; prev < 9; ++prev) {
                seq_t val = dp_prev[last][prev];
                if (val == 0) continue;

                for (int x = 0; x < 8; ++x) {
                    if (x != last) {                  /* next value is different */
                        dp_next[x][prev] += val;
                    } else {                          /* repeat */
                        if (prev == 8) {               /* first repeat seen */
                            dp_next[x][x] += val;
                        } else {
                            int diff = (last > prev) ? last - prev : prev - last;
                            if (diff > 1) {
                                dp_next[x][x] += val;
                            }
                        }
                    }
                }
            }
        }

        /* sum all states to obtain a(len) */
        seq_t total = 0;
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 9; ++j)
                total += dp_next[i][j];

        /* grow cache to hold new value */
        seq_t *new_cache = seq_alloc_new_array(len);
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        new_cache[cache_size] = total;          /* index len-1 */
        free_seq_array(cache);
        cache      = new_cache;
        cache_size = len;

        /* prepare for next iteration */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    return cache[n - 1];
}
