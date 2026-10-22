/*
 * A365104  Number of distinct quintic residues x^5 (mod 5^n), x=0..5^n-1.
 * https://oeis.org/A365104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A365104: Number of distinct quintic residues x^5 (mod 5^n)
 * Recurrence: a(0)=1, a(1)=5, a(2)=5, a(3)=21, a(4)=101, a(5)=501,
 *              for n ≥ 6: a(n) = 4*5^(n-2) + a(n-5) 
 *                (equivalently, a(n)=5*a(n-1)+a(n-5)-5*a(n-6))
 */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static idx_t cache_sz  = 0;
    static idx_t old_sz    = 0;

    /* grow the cache to hold values up to n */
    size_t new_sz = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!new_cache)          /* allocation failed */
        return (seq_t)0;
    cache = new_cache;
    old_sz = cache_sz;
    cache_sz = new_sz;

    /* incremental computation of 5^(i-2) for the recurrence */
    seq_t pow5 = 1;      /* current value of 5^pow_i */
    idx_t pow_i = 0;     /* pow5 == 5^pow_i */

    for (idx_t i = old_sz; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)5;
        else if (i == 2)
            cache[2] = (seq_t)5;
        else if (i == 3)
            cache[3] = (seq_t)21;
        else if (i == 4)
            cache[4] = (seq_t)101;
        else /* i >= 5 */
        {
            /* advance pow5 to 5^(i-2) */
            while (pow_i < i - 2) {
                pow5 *= (seq_t)5;
                ++pow_i;
            }
            cache[i] = (seq_t)4 * pow5 + cache[i - 5];
        }
    }

    return cache[n];
}
