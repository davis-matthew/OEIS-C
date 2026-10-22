/*
 * A338193  E.g.f. A(x) satisfies: A(x) = 1 + Integral (x/A(x))' / (x/A(x)^2)'
 *          dx.
 * https://oeis.org/A338193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence (2*n-1)*a(n) = (12*n^2 - 35*n +
 *            24)*a(n-1) - (n-1)*(2*n^2 - 5*n - 2)*a(n-2) +
 *            (n-4)*(n-2)*(n-1)*a(n-3) with the initial values a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* dynamic cache ----------------------------------------------------- */
static seq_t *cache = NULL;      /* array of a(n) values */
static size_t cache_cap = 0;     /* allocated capacity (indices 0..cap-1) */

/* expand the cache so that it can hold indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* initialise the first four terms of the sequence (if not already done) */
static void
init_cache(void)
{
    if (cache_cap != 0)          /* already initialised */
        return;
    ensure_cache(3);             /* capacity for indices 0..3 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)10;
}

/* compute a(n) using memoised recurrence -------------------------- */
seq_t a(idx_t n)
{
    init_cache();

    if (n < 4)
        return cache[(size_t)n];

    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* convert index to seq_t for arithmetic */
        seq_t i_seq = (seq_t)i;

        /* coefficients (all non‑negative for i >= 4) */
        seq_t c1 = (seq_t)(12 * i_seq * i_seq - 35 * i_seq + 24);
        seq_t c2 = (seq_t)((i_seq - 1) *
                    (2 * i_seq * i_seq - 5 * i_seq - 2));
        seq_t c3 = (seq_t)((i_seq - 4) *
                    (i_seq - 2) *
                    (i_seq - 1));

        /* terms of the recurrence */
        seq_t t1 = c1 * cache[i - 1];
        seq_t t2 = c2 * cache[i - 2];
        seq_t t3 = c3 * cache[i - 3];

        /* numerator and division by (2*i-1) */
        seq_t numer = t1 - t2 + t3;
        seq_t denom = (seq_t)((idx_t)2 * i - 1);

        cache[i] = numer / denom;
    }

    return cache[n];
}
