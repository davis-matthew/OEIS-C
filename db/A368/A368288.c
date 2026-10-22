/*
 * A368288  Expansion of e.g.f. exp(2*x) / (1 - log(1+x)).
 * https://oeis.org/A368288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n + Sum_{k=1..n} (-1)^(k-1) * (k-1)! * binomial(n,k) *
 *            a(n-k)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
/* Allocate array using C++ new/delete for arbitrary‑precision builds */
static inline seq_t* alloc_arr(idx_t cap) { return new seq_t[cap]; }
static inline void free_arr(seq_t* p) { delete[] p; }
#else
/* Allocate array using malloc/free for plain‑C builds */
#include <string.h>  /* for memcpy */
static inline seq_t* alloc_arr(idx_t cap) { return (seq_t*)malloc(cap * sizeof(seq_t)); }
static inline void free_arr(seq_t* p) { free(p); }
#endif

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t* cache = NULL;
    static idx_t   cache_size = 0;    /* number of values already stored               */
    static seq_t   pow2 = 0;          /* 2^i for the next index to compute (i starts at 0) */

    /* Ensure the cache can hold all needed terms 0..n */
    if (n >= cache_size) {
        /* Compute at least double the current capacity (or at least 1) */
        idx_t new_cap = (cache_size == 0) ? 1 : cache_size;
        while (new_cap <= n) new_cap <<= 1;

        /* Reallocate if the array is too small */
        if (new_cap > cache_size) {
            seq_t* new_cache = alloc_arr(new_cap);
            if (cache != NULL) {
                /* Copy old values */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
                /* C11 provides memcpy with uint64_t, but we maintain compatibility */
                memcpy(new_cache, cache, cache_size * sizeof(seq_t));
#else
                for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
#endif
                free_arr(cache);
            }
            cache = new_cache;
        }

        /* If this is the very first call initialise the first element */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
            pow2   = (seq_t)1;     /* 2^0 = 1  */
            cache_size = 1;
        }

        /* Compute missing terms from cache_size up to n inclusive */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* pow2 holds 2^(i-1) before the increment below */
            pow2 *= (seq_t)2;                  /* now pow2 = 2^i */

            seq_t sum = 0;
            /* For i > 0, evaluate the recurrence sum */
            if (i > 0) {
                seq_t fact  = (seq_t)1;       /* (k-1)!  , start with 0! = 1 for k=1   */
                seq_t binom = (seq_t)i;       /* C(i,1)  */

                for (idx_t k = 1; k <= i; ++k) {
                    seq_t term = fact * binom * cache[i - k];
                    /* (-1)^(k-1) : positive for odd k, negative for even k */
                    if ((k & 1) == 0)
                        sum -= term;
                    else
                        sum += term;

                    /* Update fact and binom for the next k, unless we are at the end */
                    if (k < i) {
                        fact  *= (seq_t)k;                      /* (k)!   */
                        binom  = binom * (seq_t)(i - k) / (seq_t)(k + 1); /* C(i,k+1) */
                    }
                }
            }

            cache[i] = sum + pow2;  /* a(i) = 2^i + sum */
            ++cache_size;           /* now we have one more stored value */
        }
    }

    return cache[n];
}
