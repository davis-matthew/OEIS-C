/*
 * A243157  Series reversion of x*(1 - x)/(1 - x - x^3).
 * https://oeis.org/A243157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 22, i32 n <= 38, i64 n <= 66, i128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243157: series reversion of x*(1 - x)/(1 - x - x^3)
   Formula (OEIS): a(n) = (1/n) * Σ_{k=0..⌊(n-1)/3⌋} C(n,k)*C(n-2k-2,n-1-3k)*(-1)^k
   Recurrence:
     -n*(n+1)*a(n) + 9*n*(n-1)*a(n-1) + 6*(-6*n^2+22*n-17)*a(n-2)
     +27*(3*n-7)*(n-4)*a(n-3) + 2*(-14*n^2+172*n-435)*a(n-4)
     -3*(53*n-192)*(n-5)*a(n-5) + 341*(n-5)*(n-6)*a(n-6) = 0
   Initial values:
     a(1)=1, a(2)=0, a(3)=0, a(4)=-1, a(5)=-1, a(6)=-1
   The function returns a(n) for any n ≥ 1 (undefined for n==0).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__SIZEOF_INT128__)
#  define HAVE_INT128 1
#else
#  define HAVE_INT128 0
#endif

/* Function prototype provided by the build system */
/* seq_t a(idx_t n); */

seq_t a(idx_t n)
{
    /* Cache to store already computed values.  */
    static seq_t *cache = NULL;        /* cache[i] holds a(i+1) */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* constant initial values for a(1)..a(6)  */
    static const seq_t init[6] = {
        (seq_t)1,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)0,   /* a(3) */
        (seq_t)-1,  /* a(4) */
        (seq_t)-1,  /* a(5) */
        (seq_t)-1   /* a(6) */
    };

    /* Safety: the sequence is defined for n ≥ 1 only.  */
    if (n == 0) return (seq_t)0;

    /* For the first six terms, answer directly.  */
    if (n <= 6) return init[n - 1];

    /* Initialize the cache with the known first six values if it is empty.  */
    if (cache_len == 0) {
        size_t need = 6;
        if (cache_cap < need) {
            size_t newcap = cache_cap ? cache_cap : 8;
            while (newcap < need) newcap <<= 1;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!cache) abort();
            cache_cap = newcap;
        }
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
    }

    /* Extend the cache up to the requested index n.  */
    size_t needed = (size_t)n;
    if (cache_len < needed) {
        if (cache_cap < needed) {
            size_t newcap = cache_cap ? cache_cap : 8;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = newcap;
        }

        for (size_t idx = cache_len; idx < needed; ++idx) {
            /* idx runs over 0..needed-1; cache[idx] corresponds to a(idx+1).  */
            idx_t k = (idx_t)(idx + 1);          /* current term index (≥7) */
            seq_t val;

#if HAVE_INT128
            if (sizeof(seq_t) <= sizeof(long long)) {
                /* Use 128‑bit arithmetic to avoid overflow on small integer types. */
                __int128 n128 = (__int128)k;
                __int128 term = (__int128)9 * n128 * (n128 - 1) *
                                (__int128)cache[idx - 1];
                term += (__int128)6 *
                        (-6 * n128 * n128 + 22 * n128 - 17) *
                        (__int128)cache[idx - 2];
                term += (__int128)27 *
                        (3 * n128 - 7) *
                        (n128 - 4) *
                        (__int128)cache[idx - 3];
                term += (__int128)2 *
                        (-14 * n128 * n128 + 172 * n128 - 435) *
                        (__int128)cache[idx - 4];
                term += (__int128)-3 *
                        (53 * n128 - 192) *
                        (n128 - 5) *
                        (__int128)cache[idx - 5];
                term += (__int128)341 *
                        (n128 - 5) *
                        (n128 - 6) *
                        (__int128)cache[idx - 6];
                term /= (n128 * (n128 + 1));
                val = (seq_t)term;
            } else
#endif
            {
                /* For arbitrary‑precision seq_t we rely on the type’s own
                   arithmetic to hold intermediate values. */
                seq_t n = (seq_t)k;
                seq_t term = (seq_t)9 * n * (n - (seq_t)1) * cache[idx - 1];

                term += (seq_t)6 *
                        ((seq_t)-6 * n * n + (seq_t)22 * n - (seq_t)17) *
                        cache[idx - 2];

                term += (seq_t)27 *
                        ((seq_t)3 * n - (seq_t)7) *
                        ((seq_t)(k - 4)) *
                        cache[idx - 3];

                term += (seq_t)2 *
                        ((seq_t)-14 * n * n + (seq_t)172 * n - (seq_t)435) *
                        cache[idx - 4];

                term += (seq_t)-3 *
                        ((seq_t)53 * n - (seq_t)192) *
                        ((seq_t)(k - 5)) *
                        cache[idx - 5];

                term += (seq_t)341 *
                        ((seq_t)(k - 5)) *
                        ((seq_t)(k - 6)) *
                        cache[idx - 6];

                term /= ((seq_t)k * ((seq_t)k + (seq_t)1));
                val = term;
            }

            cache[idx] = val;
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
