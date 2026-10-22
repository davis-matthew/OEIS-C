/*
 * A302624  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302624: a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) for n>8
   with initial values a(1)=4, a(2)=25, a(3)=25, a(4)=45,
   a(5)=79, a(6)=146, a(7)=286, a(8)=563. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset; value is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Static cache of computed values. */
    static seq_t *cache = NULL;          /* a(1) at cache[0] */
    static idx_t computed = 0;           /* number of values stored */

    /* Expand cache if necessary. */
    if (n > computed)
    {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();             /* insufficient memory */
        cache = tmp;

        /* Compute new entries in order. */
        for (idx_t i = computed + 1; i <= new_size; ++i)
        {
            idx_t idx = i - 1;          /* zero‑based index in cache */
            switch (i)
            {
                case 1:  cache[idx] = (seq_t)4;   break;
                case 2:  cache[idx] = (seq_t)25;  break;
                case 3:  cache[idx] = (seq_t)25;  break;
                case 4:  cache[idx] = (seq_t)45;  break;
                case 5:  cache[idx] = (seq_t)79;  break;
                case 6:  cache[idx] = (seq_t)146; break;
                case 7:  cache[idx] = (seq_t)286; break;
                case 8:  cache[idx] = (seq_t)563; break;
                default:
                    /* a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) */
                    seq_t a1 = cache[idx - 1]; /* a(n-1) */
                    seq_t a3 = cache[idx - 3]; /* a(n-3) */
                    seq_t a4 = cache[idx - 4]; /* a(n-4) */
                    cache[idx] = (seq_t)2 * a1 + a3 - (seq_t)2 * a4;
                    break;
            }
        }
        computed = new_size;
    }

    return cache[n - 1];
}
