/*
 * A262086  Numbers k such that phi(k + 10) = phi(k) + 10, where phi(k) =
 *          A000010(k) is Euler's totient function.
 * https://oeis.org/A262086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that phi(k + 10) = phi(k) + 10
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* Forward declaration of seq_t and idx_t are provided externally. */

/* --------------------------------------------------------------------- */
/*  Pre‑computed totient array up to a sufficiently large limit.        */
/* --------------------------------------------------------------------- */
static seq_t  *phi_arr   = NULL;   /* phi[i] holds φ(i)                              */
static idx_t   phi_limit = 5000000;/* limit (inclusive) for pre‑computation          */

/* Build the totient sieve once. */
static void init_phi_sieve(void)
{
    if (phi_arr != NULL)
        return;                     /* already initialized */

    /* Allocate array [0 .. phi_limit] */
    size_t sz = (size_t)(phi_limit + 1);
    phi_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (phi_arr == NULL)
        abort();                    /* memory allocation failed */

    /* Initialize phi[i] = i for all i */
    for (idx_t i = 0; i <= phi_limit; ++i)
        phi_arr[i] = (seq_t)i;

    /* Sieve to compute totients */
    for (idx_t p = 2; p <= phi_limit; ++p)
    {
        if (phi_arr[p] == (seq_t)p)          /* p is prime */
        {
            for (idx_t j = p; j <= phi_limit; j += p)
                phi_arr[j] -= phi_arr[j] / (seq_t)p;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Cache for the sequence terms.                                        */
/* --------------------------------------------------------------------- */
static seq_t  *cache     = NULL;   /* array holding found k values               */
static size_t cache_cnt  = 0;      /* number of terms cached                    */
static size_t cache_cap  = 0;      /* capacity of the cache array               */
static idx_t  last_k     = 0;      /* largest k already examined                */

/* Grow the cache if necessary. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap < 1024 ? 1024 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for zero index; behave gracefully */
        return 0;

    /* Initialise the totient sieve once. */
    init_phi_sieve();

    /* Initialise the cache once. */
    if (cache == NULL)
    {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache_cnt = 0;
        last_k = 0;
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_cnt < (size_t)n)
    {
        ++last_k;
        /* Ensure we do not exceed the pre‑computed limit. */
        assert(last_k + 10 <= phi_limit);

        seq_t phi_k   = phi_arr[last_k];
        seq_t phi_k10 = phi_arr[last_k + 10];

        if (phi_k10 == phi_k + (seq_t)10)
        {
            /* Extend cache if necessary. */
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt++] = last_k;
        }
    }

    return cache[n - 1];   /* 1‑based indexing into cache */
}
