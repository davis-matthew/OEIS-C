/*
 * A327927  Expansion of e.g.f. exp(Sum_{i>=1} Sum_{j=1..i} x^(i*j) / (i*j)).
 * https://oeis.org/A327927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 * Recurrence derived from the differential equation of
 *            the e.g.f.: * a(n) = Σ_{k=1}^n c(k) * a(n-k) * (n-1)_{k-1} , *
 *            where c(k)=ceil(d(k)/2) and (n-1)_{k-1}= (n-1)(n-2)…(n-k+1). *
 *            This recurrence uses only integer arithmetic in the type seq_t
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for sequence values a(0..N) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of cached entries */
    /* static arrays for divisor related data */
    static int   *spf  = NULL;          /* smallest prime factor */
    static unsigned int *dcnt = NULL;   /* divisor counts */
    static unsigned int *cval = NULL;   /* c(k) = ceil(d(k)/2) */
    static size_t data_size = 0;        /* size of data arrays */

    /* ----------  ensure data arrays up to n  ---------- */
    if (n >= data_size) {
        size_t newsize = n + 1;
        spf   = (int*)realloc(spf,   newsize * sizeof(int));
        dcnt  = (unsigned int*)realloc(dcnt, newsize * sizeof(unsigned int));
        cval  = (unsigned int*)realloc(cval, newsize * sizeof(unsigned int));
        if (!spf || !dcnt || !cval) {
            /* allocation failure – abort */
            abort();
        }

        /* simple SPF sieve freshly up to newsize-1 */
        for (size_t i = 0; i < newsize; ++i) spf[i] = 0;
        spf[0] = 0;  spf[1] = 1;
        for (size_t i = 2; i < newsize; ++i) {
            if (spf[i] == 0) {
                spf[i] = (int)i;
                if (i > newsize / i) continue;
                for (size_t j = i * i; j < newsize; j += i)
                    if (spf[j] == 0) spf[j] = (int)i;
            }
        }
        /* divisor counts and c(k) */
        dcnt[0] = dcnt[1] = 1;
        cval[0] = cval[1] = 1;
        for (size_t i = 2; i < newsize; ++i) {
            int p = spf[i];
            int e = 0;
            int m = (int)i;
            while (m % p == 0) { ++e; m /= p; }
            dcnt[i] = dcnt[m] * (e + 1);
            cval[i] = (dcnt[i] + 1) / 2;   /* ceil(d/2) */
        }
        data_size = newsize;
    }

    /* ----------  ensure sequence cache up to n  ---------- */
    if (n + 1 > cache_size) {
        size_t newsize = n + 1;
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) abort();

        /* initialise the new entries */
        if (cache_size == 0) {             /* first time */
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        for (size_t idx = cache_size; idx < newsize; ++idx) {
            seq_t sum = (seq_t)0;
            seq_t weight = (seq_t)1;      /* (idx-1)_{k-1} for k=1 */
            for (size_t k = 1; k <= idx; ++k) {
                seq_t term = cache[idx - k];
                term *= (seq_t)cval[k];
                term *= weight;
                sum += term;
                if (k < idx)                /* update weight for next k */
                    weight *= (seq_t)(idx - k);
            }
            cache[idx] = sum;
        }
        cache_size = newsize;
    }

    return cache[n];
}
