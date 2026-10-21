/*
 * A279581  Number of 2 X n 0..2 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 36*a(n-2) - 22*a(n-3) + 140*a(n-4) * -
 *            48*a(n-5) - 121*a(n-6) + 88*a(n-7) - 16*a(n-8) * Initial values:
 *            a(1)=0, a(2)=4, a(3)=36, a(4)=304, * a(5)=2212, a(6)=15428,
 *            a(7)=103648, a(8)=680052 * The index starts at 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;         /* 1‑based array of computed values */
static idx_t cache_cap = 0;         /* allocated capacity (size of 'cache') */
static idx_t cache_len = 0;         /* highest index that has been computed */

/* Initialise the cache with the first eight terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                    /* already initialised */

    cache_cap = 9;                 /* indices 0..8 inclusive */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();                   /* allocation failure */

    cache[0] = (seq_t)0;           /* unused */
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)36;
    cache[4] = (seq_t)304;
    cache[5] = (seq_t)2212;
    cache[6] = (seq_t)15428;
    cache[7] = (seq_t)103648;
    cache[8] = (seq_t)680052;

    cache_len = 8;
}

/* Ensure the cache can hold indices up to at least `need`. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap - 1)
        return;
    if (cache == NULL)
        init_cache();

    idx_t new_cap = need + 1;      /* indices 0..need inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                   /* allocation failure */

    /* Zero any newly added entries */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    cache = tmp;
    cache_cap = new_cap;
}

/* Expand the cache up to and including index `n` using the recurrence. */
static void expand_to(idx_t n)
{
    if (n <= cache_len)
        return;                    /* nothing to do */

    if (cache == NULL)
        init_cache();

    ensure_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* Compute the recurrence: a(i) = 12*a(i-1) - 36*a(i-2) - ... */
        seq_t pos = 0, neg = 0;

        pos += cache[i - 1] * (seq_t)12;
        pos += cache[i - 4] * (seq_t)140;
        pos += cache[i - 7] * (seq_t)88;

        neg += cache[i - 2] * (seq_t)36;
        neg += cache[i - 3] * (seq_t)22;
        neg += cache[i - 5] * (seq_t)48;
        neg += cache[i - 6] * (seq_t)121;
        neg += cache[i - 8] * (seq_t)16;

        cache[i] = pos - neg;
    }

    cache_len = n;
}

/* Return the nth term of the sequence, 1‑based index. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for offset 1, but safe to return 0 */

    if (cache == NULL)
        init_cache();

    if (n <= cache_len)
        return cache[n];

    expand_to(n);
    return cache[n];
}
