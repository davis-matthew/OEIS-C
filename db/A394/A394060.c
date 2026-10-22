/*
 * A394060  a(n+1) = a(n) + 2*d(a(n)) if s(a(n)) <= a(n), else 2*a(n) -
 *          s(a(n)), with a(1) = 1; d(k) is the number of divisors of k
 *          A000005 and s(k) is the aliquot sum of k A001065.
 * https://oeis.org/A394060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 477, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   calc_divisor_and_aliquot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A394060
 *
 *  a(1) = 1
 *  a(n+1) = a(n) + 2*d(a(n))                 if s(a(n)) <= a(n)
 *          2*a(n) - s(a(n))                   otherwise
 *
 *  where d(k) = number of divisors of k
 *        s(k) = aliquot sum of k  (sum of proper divisors)
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/calc_divisor_and_aliquot.h"

/* ---------- helper: factorisation of a seq_t value ---------- */

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* static cache: 1-indexed array of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0)                /* undefined, but avoid crash */
        return 0;

    /* grow cache if needed */
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;

        seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) abort();

        if (cache_len > 0)
            memcpy(new_buf, cache, cache_len * sizeof(seq_t));

        free(cache);
        cache      = new_buf;
        cache_cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
        {
            val = 1;                 /* a(1) = 1 */
        }
        else
        {
            seq_t prev = cache[i - 2];        /* a(i-1) */

            uint64_t d_val;
            seq_t    s_val;
            calc_divisor_and_aliquot(prev, &d_val, &s_val);

            if (s_val <= prev)
            {
                val = prev + (seq_t)2 * (seq_t)d_val;   /* a + 2*d */
            }
            else
            {
                val = (seq_t)2 * prev - s_val;          /* 2*a - s */
            }
        }
        cache[i - 1] = val;
    }

    cache_len = n > cache_len ? n : cache_len;
    return cache[n - 1];
}
