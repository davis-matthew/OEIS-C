/*
 * A329707  Number of placements of zero or more dominoes on a 2 X n grid
 *          where no two empty squares are horizontally adjacent.
 * https://oeis.org/A329707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-6) for n > 6 with a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=11, a(4)=25, a(5)=61, a(6)=146
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure, caller may treat as zero */
        cache = tmp;

        for (idx_t i = cache_size; i < new_size; ++i)
        {
            switch (i)
            {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)2; break;
                case 2: cache[2] = (seq_t)4; break;
                case 3: cache[3] = (seq_t)11; break;
                case 4: cache[4] = (seq_t)25; break;
                case 5: cache[5] = (seq_t)61; break;
                case 6: cache[6] = (seq_t)146; break;
                default:
                    cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2] - cache[i - 6];
                    break;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
