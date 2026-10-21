/*
 * A286350  a(n) = 2*a(n-1) - a(n-2) + a(n-4) for n>3, a(0)=0, a(1)=a(2)=2,
 *          a(3)=3.
 * https://oeis.org/A286350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4), with a(0)=0, a(1)=2, a(2)=2,
 *            a(3)=3
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Memoized iterative computation of the recurrence             */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;         /* array holding computed terms          */
static idx_t  cache_size = 0;       /* allocated capacity (in terms)         */
static idx_t  last_computed = 0;    /* highest index that has a defined term */

static void init_cache(void)
{
    /* start with space for the first four seeds */
    cache_size = 4;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache) abort(); /* allocation failure – abort */

    cache[0] = 0;
    cache[1] = 2;
    cache[2] = 2;
    cache[3] = 3;
    last_computed = 3;
}

static void grow_cache(idx_t n)
{
    /* ensure capacity for indices up to n (inclusive) */
    if (cache_size > n) return;           /* already big enough */

    idx_t new_size = cache_size;
    if (new_size == 0) new_size = 4;      /* initial size */

    while (new_size <= n) new_size <<= 1; /* double until>n */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure – abort */

    cache = tmp;
    cache_size = new_size;
}

/* ------------------------------------------------------------ */
/*  Public function implementing the sequence                   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* first call – initialise the cache */
    if (cache == NULL)
        init_cache();

    /* grow and compute up to n if necessary */
    if (n > last_computed)
    {
        grow_cache(n);

        /* compute missing terms from last_computed+1 up to n */
        idx_t i;
        for (i = last_computed + 1; i <= n; ++i)
        {
            if (i < 4) /* shouldn't happen – seeds already set */
                continue;

            /* a(i) = 2*a(i-1) - a(i-2) + a(i-4) */
            cache[i] = 2 * cache[i - 1] - cache[i - 2] + cache[i - 4];
        }
        last_computed = n;
    }

    /* return the requested term */
    return cache[n];
}
