/*
 * A280197  Expansion of 1/(1 - Sum_{k>=2} mu(k)^2*x^k), where mu(k) is the
 *          Moebius function (A008683).
 * https://oeis.org/A280197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into squarefree parts ≥ 2
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 55, u64 n <= 108,
 *            u128 n <= 213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine memoises the required values in static tables that grow
   on demand.  The square‑free table is created by a simple sieve: every
   multiple of p² (p ≥ 2) is marked as non‑square‑free.  The recurrence
   a(0) = 1, a(1) = 0, and for n ≥ 2,
        a(n) = Σ_{k=2}^{n} is_sqf(k) * a(n‑k)   (a long‑hand
   convolution of the square‑free indicator with the sequence itself). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* static (per‑process) state: cache of computed values                */
/* -------------------------------------------------------------------- */
static seq_t *cache = NULL;       /* memoised a(n) values                 */
static size_t cache_cap = 0;      /* allocated size of the cache          */
static idx_t  computed_max = 0;   /* largest n for which a(n) is known    */

/* -------------------------------------------------------------------- */
/* static (per‑process) state: square‑free indicator table              */
/* -------------------------------------------------------------------- */
static unsigned char *sqf = NULL; /* 1 if i is squarefree ≥ 2, else 0       */
static size_t sqf_cap = 0;        /* allocated size of the sqf table      */

/* -------------------------------------------------------------------- */
/* ensure that the square‑free table is valid up to 'up_to' inclusive   */
/* -------------------------------------------------------------------- */
static void ensure_squarefree(idx_t up_to)
{
    if (up_to + 1 <= sqf_cap)                 /* already up to date     */
        return;

    size_t old = sqf_cap;
    sqf_cap = (size_t)(up_to + 1);

    unsigned char *tmp = (unsigned char*)realloc(sqf, sqf_cap);
    if (tmp == NULL)
        abort();                                /* allocation failure     */
    sqf = tmp;

    /* initialise the newly added entries to 1 (possibly square‑free) */
    memset(sqf + old, 1, sqf_cap - old);

    /* 0 and 1 are not allowed as parts                           */
    if (sqf_cap > 0) sqf[0] = 0;
    if (sqf_cap > 1) sqf[1] = 0;

    /* mark non‑square‑free numbers via the sieve of p²             */
    for (idx_t p = 2; (idx_t)p * p <= up_to; ++p) {
        if (sqf[p]) {                           /* p itself is square‑free */
            idx_t pp = p * p;
            for (idx_t m = pp; m <= up_to; m += pp)
                sqf[m] = 0;
        }
    }
}

/* -------------------------------------------------------------------- */
/* resize the cache so that it can hold indices up to 'n' (inclusive)  */
/* -------------------------------------------------------------------- */
static void ensure_cache_cap(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                                /* allocation failure     */
    cache = tmp;

    /* zero the newly allocated cells                           */
    memset(cache + cache_cap,
           0,
           (new_cap - cache_cap) * sizeof(seq_t));

    cache_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/* public routine: sequence term a(n)                                  */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* special‑case the first two terms */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache = (seq_t*)calloc(2, sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache_cap = 2;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        computed_max = 1;
    }

    /* make sure space for the requested index exists */
    ensure_cache_cap(n);

    /* already known value */
    if (n <= computed_max)
        return cache[n];

    /* ensure the square‑free table covers the needed range */
    ensure_squarefree(n);

    /* compute all missing values up to n (dynamic programming) */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        for (idx_t k = 2; k <= cur; ++k) {
            if (sqf[(size_t)k])         /* k is a legal part */
                sum += cache[cur - k];
        }
        cache[cur] = sum;
    }
    computed_max = n;
    return cache[n];
}
