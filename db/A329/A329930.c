/*
 * A329930  a(n) = n!^2*(Sum_{k=1..n} 1/k).
 * https://oeis.org/A329930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^2*(Sum_{k=1..n} 1/k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 0, a(1) = 1, a(2) = 6, ... */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for factorial squares and the sequence values */
    static seq_t *fact_sq = NULL;   /* fact_sq[i] = (i!)^2          */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)            */
    static size_t cache_len = 0;    /* first unused index           */

    /* Ensure the caches are big enough for index n */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        /* Reallocate the arrays; malloc/realloc is safe for C and works for
           trivially‑constructible big‑int types. */
        seq_t *tmp_fact = (seq_t *)realloc(fact_sq, new_len * sizeof(seq_t));
        seq_t *tmp_a    = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp_fact || !tmp_a) /* allocation failed */
            abort();
        fact_sq = tmp_fact;
        a_cache = tmp_a;

        /* Initialise fresh slots */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)
            {
                fact_sq[0] = 1;      /* 0!^2 = 1  */
                a_cache[0] = 0;      /* a(0) = 0   */
            }
            else
            {
                seq_t i_seq = (seq_t)i;
                /* fact_sq[i] = fact_sq[i-1] * i^2  */
                fact_sq[i] = fact_sq[i-1] * i_seq * i_seq;
                /* a(i) = a(i-1)*i^2 + fact_sq[i-1]*i */
                a_cache[i] = a_cache[i-1] * i_seq * i_seq + fact_sq[i-1] * i_seq;
            }
        }
        cache_len = new_len;
    }

    return a_cache[(size_t)n];
}
