/*
 * A250126  Coordination sequence of point of type 3.3.4.12 in 4-uniform
 *          tiling {3.3.4.3.4; 3.3.4.12; 3.3.12.4; 3.4.3.12}.
 * https://oeis.org/A250126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n>=5? a(n-5):0) + (n>=7? a(n-7):0) - (n>=12? a(n-12):0)
 *            + b(n), where b(n) is 1 for n=0 4 for n=1 9 for n=2 9 for n=3 12
 *            for n=4 18 for n=5 17 for n=6 18 for n=7 14 for n=8 10 for n=9
 *            10 for n=10 7 for n=11 2 for n=12 0 for n=13 -1 for n=14 0 for
 *            n=15 -2 for n=16 0 for n>=17
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Extend the cache to contain value at index n */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Compute values lazy, from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 5) val += cache[i - 5];
            if (i >= 7) val += cache[i - 7];
            if (i >= 12) val -= cache[i - 12];

            /* add b(i) */
            switch (i)
            {
                case 0:  val += (seq_t)1;  break;
                case 1:  val += (seq_t)4;  break;
                case 2:  val += (seq_t)9;  break;
                case 3:  val += (seq_t)9;  break;
                case 4:  val += (seq_t)12; break;
                case 5:  val += (seq_t)18; break;
                case 6:  val += (seq_t)17; break;
                case 7:  val += (seq_t)18; break;
                case 8:  val += (seq_t)14; break;
                case 9:  val += (seq_t)10; break;
                case 10: val += (seq_t)10; break;
                case 11: val += (seq_t)7;  break;
                case 12: val += (seq_t)2;  break;
                case 14: val -= (seq_t)1;  break; /* b(14) = -1 */
                case 16: val -= (seq_t)2;  break; /* b(16) = -2 */
                default: break; /* b(i)=0 for other i */
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
