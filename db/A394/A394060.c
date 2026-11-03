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
 * Validated: standard, pass, 72 terms through n = 1919 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

static void
calc_divisor_and_aliquot(seq_t n, uint64_t *d_out, seq_t *s_out)
{
    seq_t original = n;                     /* keep original value */
    seq_t tmp      = n;
    uint64_t d     = 1;                     /* divisor count */
    seq_t   sigma  = 1;                     /* sum of all divisors */

    /* trial division */
    for (uint64_t p = 2ULL; (seq_t)p * (seq_t)p <= tmp; ++p)
    {
        if (tmp % (seq_t)p == 0)
        {
            /* k = p^e * rest  (e >= 1) */
            uint64_t e = 0;
            while (tmp % (seq_t)p == 0)
            {
                tmp /= (seq_t)p;
                ++e;
            }
            d *= (e + 1);

            /* contribution to sigma: 1 + p + p^2 + ... + p^e
             * = (p^{e+1} - 1) / (p - 1)
             */
            seq_t p_pow = 1;
            for (uint64_t i = 0; i < e + 1; ++i)
                p_pow *= (seq_t)p;              /* p^{e+1} */
            seq_t factor = (p_pow - 1) / ((seq_t)p - 1);
            sigma *= factor;
        }
    }

    /* remaining prime factor > sqrt(original) */
    if (tmp > 1)
    {
        d *= 2;
        sigma *= (1 + tmp);
    }

    *d_out = d;
    *s_out = sigma - original;                /* aliquot sum = sigma - k */
}

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

        seq_t *new_buf = mem_new(seq_t, new_cap);
        if (!new_buf) abort();

        if (cache_len > 0)
            mem_copy(new_buf, cache, cache_len * sizeof(seq_t));

        mem_free(cache);
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
