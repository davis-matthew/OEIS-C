/*
 * A372018  G.f. A(x) satisfies A(x) = ( 1 + 4*x*A(x)/(1 - x*A(x)) )^(1/2).
 * https://oeis.org/A372018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372018
   a(n) satisfies the recurrence
       n*(n+1)*(n-2)*a(n) = 6*(n-2)*(3*n^2-6*n+1)*a(n-2)
                           + 27*n*(n-3)*(n-4)*a(n-4)
   with initial values a(0)=1, a(1)=2, a(2)=4, a(3)=10. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoization */
static seq_t *cache      = NULL;  /* array holding computed terms */
static size_t cache_cap  = 0;     /* current number of allocated slots   */
static bool   cache_init = false;/* whether the first four terms exist   */
static idx_t  last_comp  = 0;     /* largest index already computed      */

/* ensure that the cache can hold at least (need+1) entries */
static void ensure_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need + 1)
        return;

    size_t new_cap = (size_t)need + 1;
    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;
}

/* initialise the first four terms – only once */
static void init_base(void)
{
    if (cache_init)
        return;

    /* make sure we have space for indices 0..3 */
    init_base: ;
    ensure_capacity(3);
    cache[0] = 1;
    cache[1] = 2;
    cache[2] = 4;
    cache[3] = 10;
    cache_init = true;
    last_comp = 3;
}

/* main routine */
seq_t a(idx_t n)
{
    /* indices below 0 are undefined; the sequence is defined for n>=0 */
    /* Handle the base cases directly – no memoisation needed. */
    if (n <= 3) {
        init_base();                  /* just in case */
        return cache[(size_t)n];
    }

    /* ensure the cache can store up to n */
    ensure_capacity(n);
    init_base();

    /* compute missing terms up to *n* */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        /* term1 = 6*(i-2)*(3*i*i-6*i+1)*a(i-2) */
        seq_t term1 = cache[i - 2];
        term1 = term1 * (seq_t)6;
        term1 = term1 * (seq_t)(i - 2);
        idx_t tmp = 3 * i * i - 6 * i + 1;   /* fits in idx_t for our limits */
        term1 = term1 * (seq_t)tmp;

        /* term2 = 27*i*(i-3)*(i-4)*a(i-4) */
        seq_t term2 = cache[i - 4];
        term2 = term2 * (seq_t)27;
        term2 = term2 * (seq_t)i;
        term2 = term2 * (seq_t)(i - 3);
        term2 = term2 * (seq_t)(i - 4);

        /* a(i) = (term1 + term2) / (i*(i+1)*(i-2)) */
        seq_t numer   = term1 + term2;
        idx_t denom   = i * (i + 1) * (i - 2);
        cache[(size_t)i] = numer / denom;
    }

    last_comp = n;
    return cache[(size_t)n];
}
