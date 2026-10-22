/*
 * A348290  a(n) = Sum_{k=0..floor(n/10)} binomial(n-5*k,5*k).
 * https://oeis.org/A348290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/10)} binomial(n-5*k,5*k) (recurrence:
 *            a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) +
 *            a(n-10), with a(0)=…=a(9)=1, a(10)=2)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 50, u64 n <= 96, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache ----------------------------------------------------------- */
static seq_t *cache     = NULL;   /* array of previously computed terms   */
static size_t cache_cap = 0;      /* allocated capacity (elements)        */
static size_t cache_len = 0;      /* current length (next free index)     */

/* ensure capacity of the cache ------------------------------------------ */
static void ensure_capacity(size_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache for n < 10 and n == 10 ------------------------------ */
static void init_cache(void)
{
    if (cache_len > 0)
        return;                 /* already initialised                */
    ensure_capacity(15);         /* a little headroom                  */

    /* a(0)..a(9) = 1 */
    for (size_t i = 0; i <= 9; ++i)
        cache[i] = (seq_t)1;
    cache[10] = (seq_t)2;        /* a(10) = 2 */
    cache_len = 11;              /* we have indices 0..10  (next free 11) */
}

/* compute next term using the recurrence ------------------------------- */
static seq_t next_term(size_t idx)
{
    /* idx >= 11 */
    seq_t v = 5 * cache[idx - 1];
    v -= 10 * cache[idx - 2];
    v += 10 * cache[idx - 3];
    v -= 5 * cache[idx - 4];
    v += cache[idx - 5];
    v += cache[idx - 10];
    return v;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 10)                       /* a(0)..a(9) = 1 */
        return (seq_t)1;
    if (n == 10)                      /* a(10) = 2      */
        return (seq_t)2;

    init_cache();                     /* ensure cache exists */

    /* compute terms up to n if necessary */
    while (cache_len <= (size_t)n)
    {
        ensure_capacity(cache_len);
        cache[cache_len] = next_term(cache_len);
        ++cache_len;
    }

    return cache[(size_t)n];
}
