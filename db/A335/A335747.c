/*
 * A335747  Number of ways to tile vertically-fault-free 3 X n strip with
 *          squares and dominoes.
 * https://oeis.org/A335747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - a(n-3) - a(n-4) for n >= 7. Initial
 *            values: a(0)=1, a(1)=3, a(2)=13, a(3)=26, a(4)=66, a(5)=154,
 *            a(6)=380
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

static void grow_cache(idx_t needed)
{
    if (seq_cache == NULL) {
        seq_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (seq_cache == NULL) abort();
        cache_size = needed;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(seq_cache, needed * sizeof(seq_t));
        if (tmp == NULL) abort();
        seq_cache = tmp;
        cache_size = needed;
    }
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        grow_cache(n + 1);
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) val = (seq_t)1;
            else if (i == 1) val = (seq_t)3;
            else if (i == 2) val = (seq_t)13;
            else if (i == 3) val = (seq_t)26;
            else if (i == 4) val = (seq_t)66;
            else if (i == 5) val = (seq_t)154;
            else if (i == 6) val = (seq_t)380;
            else {
                val = seq_cache[i-1] + (seq_t)4 * seq_cache[i-2]
                    - seq_cache[i-3] - seq_cache[i-4];
            }
            seq_cache[i] = val;
        }
        cache_size = n + 1;
    }
    return seq_cache[n];
}
