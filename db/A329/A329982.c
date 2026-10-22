/*
 * A329982  a(1) = 0, and for n > 0, a(n+1) = k^2 - a(n) where k is the number
 *          of terms equal to a(n) among the first n terms.
 * https://oeis.org/A329982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 0; for n >= 1, a(n+1) = k^2 – a(n) where k is the number
 *            of times a(n) occurs among the first n terms (the ordinal
 *            transform of a(n))
 * Range:     i8 n <= 937, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: a[0] is a(1), a[1] is a(2), etc. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Auxiliary tables to keep distinct values and their counts so we can
       obtain k in O(1) per step. */
    static seq_t *values = NULL;
    static size_t  *counts = NULL;
    static size_t  val_len = 0, val_cap = 0;

    /* initialise storage on first call */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

        val_cap = 16;
        values = (seq_t*)malloc(val_cap * sizeof(seq_t));
        counts = (size_t*)malloc(val_cap * sizeof(size_t));
    }

    /* insert initial value a(1) = 0 if cache is still empty */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)0;
        cache_len = 1;

        values[0] = (seq_t)0;
        counts[0] = 1;
        val_len  = 1;
    }

    /* shortcut: already computed */
    if (n <= (idx_t)cache_len)
        return cache[(size_t)(n - 1)];

    /* extend cache up to index n */
    while (cache_len < n)
    {
        /* previous term a(current index) where current index = cache_len (1‑based) */
        seq_t prev = cache[cache_len - 1];

        /* find k = occurrences of prev */
        size_t k = 0;
        for (size_t i = 0; i < val_len; ++i)
        {
            if (values[i] == prev)
            {
                k = counts[i];
                break;
            }
        }

        /* a(next) = k^2 – prev */
        seq_t next = (seq_t)k * (seq_t)k - prev;

        /* grow cache if necessary */
        if (cache_len == cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = next;
        ++cache_len;

        /* update value/counter tables for next step */
        size_t idx;
        int found = 0;
        for (idx = 0; idx < val_len; ++idx)
        {
            if (values[idx] == next)
            {
                ++counts[idx];
                found = 1;
                break;
            }
        }
        if (!found)
        {
            if (val_len == val_cap)
            {
                val_cap *= 2;
                values = (seq_t*)realloc(values, val_cap * sizeof(seq_t));
                counts = (size_t*)realloc(counts, val_cap * sizeof(size_t));
            }
            values[val_len] = next;
            counts[val_len] = 1;
            ++val_len;
        }
    }

    return cache[(size_t)(n - 1)];
}
