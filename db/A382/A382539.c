/*
 * A382539  Expansion of 1/(1 - x/(1 - 4*x)^(1/2))^2.
 * https://oeis.org/A382539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{k=0}^{n} 4^{\,n-k}\,(k+1)\binom{(n-k)/2-1}{\,n-k}
 *            Recurrence:
 *            (-n+1)a(n)+2(4n-7)a(n-1)+(-15n+41)a(n-2)+2(-2n+3)a(n-3)=0 Base
 *            values: a(0)=1, a(1)=2, a(2)=7. This function returns exact a(n)
 *            for all n≥0 and memoises values
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _SEQ_A382539_H_
#define _SEQ_A382539_H_

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* If seq_t or idx_t are not defined, the build provides them. */
/* ------------------------------------------------------------------ */

#ifdef __cplusplus

/* C++ implementation – seq_t may be an arbitrary‑precision integer type. */
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)7;

    if (n < cache.size())
        return cache[(size_t)n];

    cache.resize((size_t)n + 1);
    for (idx_t i = 3; i <= n; ++i)
    {
        int64_t coeff1 = 2 * ((int64_t)4 * (int64_t)i - 7);
        int64_t coeff2 = -15 * (int64_t)i + 41;
        int64_t coeff3 = 2 * (-2 * (int64_t)i + 3);

        seq_t term1 = cache[(size_t)i - 1] * (seq_t)coeff1;
        seq_t term2 = cache[(size_t)i - 2] * (seq_t)coeff2;
        seq_t term3 = cache[(size_t)i - 3] * (seq_t)coeff3;

        seq_t numerator = term1 + term2 + term3;
        cache[(size_t)i] = numerator / (seq_t)(i - 1);
    }
    return cache[(size_t)n];
}

#else   /* C implementation – seq_t is an unsigned integer type */

seq_t a(idx_t n)
{
    /* Dynamic cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t  cache_len = 0;     /* number of terms stored */

    /* Ensure sufficient capacity. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[(size_t)i] = (seq_t)1;
        }
        else if (i == 1)
        {
            cache[(size_t)i] = (seq_t)2;
        }
        else if (i == 2)
        {
            cache[(size_t)i] = (seq_t)7;
        }
        else
        {
            int64_t coeff1 = 2 * ((int64_t)4 * (int64_t)i - 7);
            int64_t coeff2 = -15 * (int64_t)i + 41;
            int64_t coeff3 = 2 * (-2 * (int64_t)i + 3);

            /* Use signed 128‑bit intermediate to avoid overflow. */
            __int128 t1 = (__int128)(unsigned long long)cache[(size_t)i - 1] * (__int128)coeff1;
            __int128 t2 = (__int128)(unsigned long long)cache[(size_t)i - 2] * (__int128)coeff2;
            __int128 t3 = (__int128)(unsigned long long)cache[(size_t)i - 3] * (__int128)coeff3;

            __int128 numer = t1 + t2 + t3;
            cache[(size_t)i] = (seq_t)(numer / (i - 1));
        }
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}

#endif  /* __cplusplus */

#endif  /* _SEQ_A382539_H_ */
