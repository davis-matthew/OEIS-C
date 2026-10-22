/*
 * A339413  a(0) = 0; for n > 0, a(n) = a(n-1) if c0 == c1; a(n) = a(n-1) - c0
 *          if c0 > c1; a(n) = a(n - 1) + c1 if c1 > c0, where c0 and c1 are
 *          respectively the number of 0's and 1's in the binary expansion of
 *          n.
 * https://oeis.org/A339413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 230, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0.
   For n>0:
     let c1 = number of 1-bits in binary n,
         c0 = length of binary n – c1 (number of 0-bits).
     if c0==c1   : a(n)=a(n-1)
     if c0>c1    : a(n)=a(n-1)-c0
     if c1>c0    : a(n)=a(n-1)+c1
   The sequence is computed on demand and cached for future calls. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>   /* placement new for seq_t construction */
#endif

/* static cache of previously computed terms */
static seq_t *cache = NULL;          /* array of terms */
static idx_t   cache_max = 0;        /* highest n computed (>=0) */
static size_t  cache_cap = 0;        /* allocated capacity */

/* internal helper: ensures cache has room up to n, and fills new entries */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max)
        return;                     /* already computed */

    /* extend the cache array if necessary */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (p == NULL)
            abort();                 /* cannot proceed */
        cache = p;
        /* construct new elements if seq_t might require a constructor */
#ifdef __cplusplus
        for (size_t i = cache_cap; i < new_cap; ++i)
            new(&cache[i]) seq_t();
#endif
        cache_cap = new_cap;
    }

    /* compute new entries */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        unsigned long long val = (unsigned long long)i;       /* for bit ops */
        unsigned int pop   = __builtin_popcountll(val);        /* 1-bits */
        unsigned int len   = 64 - __builtin_clzll(val);         /* bit length */
        unsigned int c1 = pop;
        unsigned int c0 = len - pop;

        seq_t prev = cache[i - 1];
        seq_t cur;
        if (c0 == c1)
            cur = prev;
        else if (c0 > c1)
            cur = prev - (seq_t)c0;
        else
            cur = prev + (seq_t)c1;

        cache[i] = cur;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    /* first call initialises cache[0] if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache[0] = (seq_t)0;
        cache_cap = 1;
        cache_max = 0;
    }

    if (n == 0)
        return (seq_t)0;

    compute_up_to(n);
    return cache[n];
}
