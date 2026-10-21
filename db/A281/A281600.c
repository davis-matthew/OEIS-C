/*
 * A281600  Number of n X 3 0..2 arrays with no element equal to more than one
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281600: Number of n×3 0..2 arrays with the stated property
   Recurrence (n > 6):
        a(n) = 7*a(n‑1) + 10*a(n‑2) – 26*a(n‑3) – 64*a(n‑4) – 40*a(n‑5)
   Initial terms:
        a(1)=4, a(2)=29, a(3)=209, a(4)=1623, a(5)=12413, a(6)=95623
*/

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Handle indices below the offset; unused but harmless. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache to allow out‑of‑order queries. */
    static seq_t *cache = NULL;      /* array of terms, index 0 unused */
    static size_t capacity = 0;      /* allocated size (≥ n+1) */
    static idx_t  max_n = 0;         /* highest index computed */

    /* Grow the cache if necessary. */
    if (n > max_n)
    {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > capacity)
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)  /* allocation failure – abort */
                return (seq_t)0;  /* for safety; normally should never happen */
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute terms up to n. */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            switch (i)
            {
                case 1: cache[i] = (seq_t)4;   break;
                case 2: cache[i] = (seq_t)29;  break;
                case 3: cache[i] = (seq_t)209; break;
                case 4: cache[i] = (seq_t)1623; break;
                case 5: cache[i] = (seq_t)12413; break;
                case 6: cache[i] = (seq_t)95623; break;
                default:
                {
                    /* recurrence for i >= 7 */
                    seq_t term1 = 7 * cache[i - 1];
                    term1 += 10 * cache[i - 2];

                    seq_t term2 = 26 * cache[i - 3];
                    term2 += 64 * cache[i - 4];
                    term2 += 40 * cache[i - 5];

                    cache[i] = term1 - term2;   /* guaranteed non‑negative */
                }
            }
        }

        max_n = n;
    }

    return cache[n];
}
