/*
 * A365293  a(n) = n!*tetranacci(n+3).
 * https://oeis.org/A365293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A365293
   Formula: a(n) = n!*tetranacci(n+3)
   where tetranacci is the sequence with
   T[0]=T[1]=T[2]=0, T[3]=1 and
   T[i] = T[i-1]+T[i-2]+T[i-3]+T[i-4] for i≥4. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static factorial and tetranacci caches */
    static seq_t *fact_cache      = NULL;
    static size_t fact_cache_size = 0;   /* number of elements stored */
    static seq_t *tetra_cache     = NULL;
    static size_t tetra_cache_size = 0;
    /* needed lengths for the current call */
    size_t need_fact = (size_t)n;
    size_t need_tetra = (size_t)(n + 3);

    /* grow factorial cache if required */
    if (fact_cache_size <= need_fact)
    {
        size_t new_size = fact_cache_size == 0 ? 1 : fact_cache_size;
        while (new_size <= need_fact) new_size <<= 1;

        seq_t *tmp = (seq_t *)realloc(fact_cache,
                                      new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        fact_cache = tmp;

        for (size_t i = fact_cache_size; i < new_size; ++i)
        {
            if (i == 0)
                fact_cache[i] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = new_size;
    }

    /* grow tetranacci cache if required */
    if (tetra_cache_size <= need_tetra)
    {
        size_t new_size = tetra_cache_size == 0 ? 4 : tetra_cache_size;
        while (new_size <= need_tetra) new_size <<= 1;

        seq_t *tmp = (seq_t *)realloc(tetra_cache,
                                      new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        tetra_cache = tmp;

        for (size_t i = tetra_cache_size; i < new_size; ++i)
        {
            if (i == 0 || i == 1 || i == 2)
                tetra_cache[i] = (seq_t)0;
            else if (i == 3)
                tetra_cache[i] = (seq_t)1;
            else
                tetra_cache[i] = tetra_cache[i - 1] + tetra_cache[i - 2]
                               + tetra_cache[i - 3] + tetra_cache[i - 4];
        }
        tetra_cache_size = new_size;
    }

    /* return n! * tetranacci(n+3) */
    return fact_cache[n] * tetra_cache[n + 3];
}
