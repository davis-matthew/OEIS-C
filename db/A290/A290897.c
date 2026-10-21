/*
 * A290897  p-INVERT of the positive integers, where p(S) = 1 - S - S^3.
 * https://oeis.org/A290897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * A290897 – p‑INVERT of the positive integers (p(S) = 1 – S – S³).
 * Recurrence (Offset 0):
 *   a(n) =  7*a(n‑1) – 19*a(n‑2) + 27*a(n‑3) – 19*a(n‑4) +  7*a(n‑5) – a(n‑6)
 * Initial terms:
 *   a(0)=1, a(1)=3, a(2)=9, a(3)=29, a(4)=95, a(5)=307
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* allocated array of terms */
static idx_t allocated = 0;          /* number of elements allocated */
static idx_t computed  = 0;          /* number of elements already computed */

/* internal helper – extend cache and compute new terms up to ‘upto’ (exclusive) */
static void fill_cache(idx_t upto)
{
    for (idx_t i = computed; i < upto; ++i)
    {
        if (i <= 5)                     /* initial values */
        {
            switch (i)
            {
                case 0: cache[0] = (seq_t)1;   break;
                case 1: cache[1] = (seq_t)3;   break;
                case 2: cache[2] = (seq_t)9;   break;
                case 3: cache[3] = (seq_t)29;  break;
                case 4: cache[4] = (seq_t)95;  break;
                case 5: cache[5] = (seq_t)307; break;
            }
            continue;
        }

        /* recurrence for i ≥ 6 */
        cache[i] =  7 * cache[i-1]
                  -19 * cache[i-2]
                  +27 * cache[i-3]
                  -19 * cache[i-4]
                  + 7 * cache[i-5]
                  -  cache[i-6];
    }
    computed = upto;
}

/* public entry point – returns the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* ensure the array is large enough */
    if (allocated < n + 1)
    {
        idx_t new_alloc = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* out of memory */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute missing terms if needed */
    if (computed <= n)
        fill_cache(n + 1);

    return cache[n];
}
