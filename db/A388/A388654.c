/*
 * A388654  Elements of an interval of natural numbers whose product is
 *          divisible by the square of the largest prime factor.
 * https://oeis.org/A388654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: computes A388654 – numbers that belong to a “bad” interval. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *seq_vals = NULL;
    static size_t seq_len  = 0;
    static idx_t  seq_bound = 0;   /* largest number considered so far */

    if (n == 0)   /* not defined; return zero to avoid UB */
        return 0;

    /* If we already computed enough terms, just return the cached one */
    if (seq_len >= (size_t)n)
        return seq_vals[n - 1];

    /* Choose an initial bound and enlarge if necessary */
    idx_t bound = (seq_bound > 0) ? (seq_bound * 2) : ((idx_t)n * 5);
    if (bound < 100)
        bound = 100;

    while (1)
    {
        /* --------------------- sieve for largest prime factors --------------------- */
        uint32_t *lpf   = (uint32_t *)calloc((size_t)bound + 1, sizeof(uint32_t));
        uint32_t *primes = (uint32_t *)malloc((size_t)bound + 1 * sizeof(uint32_t));
        if (!lpf || !primes)
        {
            free(lpf); free(primes);
            return 0;   /* allocation failure */
        }

        size_t primes_cnt = 0;
        for (uint32_t i = 2; i <= (uint32_t)bound; ++i)
        {
            if (lpf[i] == 0)                /* i is prime */
            {
                for (uint32_t m = i; m <= (uint32_t)bound; m += i)
                    lpf[m] = i;             /* overwrite with larger prime */
                primes[primes_cnt++] = i;
            }
        }

        /* Only primes <= sqrt(bound) can produce p^2 <= bound */
        uint32_t lim = (uint32_t)sqrt((double)bound);
        size_t sq_cnt = 0;
        while (sq_cnt < primes_cnt && primes[sq_cnt] <= lim)
            ++sq_cnt;

        /* --------------------- mark bad numbers --------------------- */
        bool *bad = (bool *)calloc((size_t)bound + 1, sizeof(bool));
        if (!bad)
        {
            free(lpf); free(primes);
            return 0;
        }

        for (size_t pi = 0; pi < sq_cnt; ++pi)
        {
            uint32_t p  = primes[pi];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;   /* p^2, avoid overflow */

            bool seg_bad  = false;
            uint32_t seg_start = 1;

            for (uint32_t i = 1; i <= (uint32_t)bound; ++i)
            {
                if (lpf[i] <= p)
                {
                    if (((uint64_t)i % p2) == 0)
                        seg_bad = true;
                }
                else   /* run ends */
                {
                    if (seg_bad)
                    {
                        for (uint32_t j = seg_start; j < i; ++j)
                            bad[j] = true;
                    }
                    seg_start = i + 1;
                    seg_bad   = false;
                }
            }
            /* close the last run */
            if (seg_bad)
            {
                for (uint32_t j = seg_start; j <= (uint32_t)bound; ++j)
                    bad[j] = true;
            }
        }

        /* --------------------- collect bad numbers into array --------------------- */
        size_t est = (bound / 5) + 10;          /* rough estimate of count */
        seq_t *tmp = (seq_t *)malloc(est * sizeof(seq_t));
        if (!tmp)
        {
            free(lpf); free(primes); free(bad);
            return 0;
        }

        size_t m = 0;
        for (uint32_t i = 1; i <= (uint32_t)bound; ++i)
        {
            if (bad[i])
            {
                if (m == est)
                {
                    est *= 2;
                    tmp = (seq_t *)realloc(tmp, est * sizeof(seq_t));
                    if (!tmp)
                    {
                        free(lpf); free(primes); free(bad);
                        return 0;
                    }
                }
                tmp[m++] = (seq_t)i;
            }
        }

        /* free temporary arrays */
        free(lpf); free(primes); free(bad);

        /* If we have enough terms, store them and return the answer */
        if (m >= (size_t)n)
        {
            seq_vals = (seq_t *)realloc(seq_vals, m * sizeof(seq_t));
            memcpy(seq_vals, tmp, m * sizeof(seq_t));
            free(tmp);
            seq_len   = m;
            seq_bound = bound;
            return seq_vals[n - 1];
        }

        /* Not enough terms yet – enlarge bound and repeat */
        free(tmp);
        bound *= 2;
    }
}
