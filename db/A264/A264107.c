/*
 * A264107  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,1.
 * https://oeis.org/A264107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A264107
    Recurrence:  a(n) = 19·a(n‑1) + 304·a(n‑3) + 256·a(n‑4)  for n ≥ 5
    Initial values (n = 1..4):
        a(1) = 144
        a(2) = 2745
        a(3) = 54756
        a(4) = 1086300
 */
#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------------- */
/*  Memoised recurrence implementation                                 */
/* -------------------------------------------------------------------- */
static seq_t *cache   = NULL;    /* cache[0] unused; cache[1] = a(1) ...  */
static idx_t  cache_len = 0;     /* largest index currently stored      */

static const seq_t C19   = (seq_t)19;
static const seq_t C304  = (seq_t)304;
static const seq_t C256  = (seq_t)256;
static const seq_t init_vals[5] = {
    0ULL,            /* unused, keeps indices 1‑based */
    144ULL,          /* a(1) */
    2745ULL,         /* a(2) */
    54756ULL,        /* a(3) */
    1086300ULL       /* a(4) */
};

/* Grow the cache array to hold indices up to 'n' inclusive.
   New elements are set to zero (they will be overwritten later).  */
static void grow_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        abort();                 /* required by the harness            */

    /* Initialise newly allocated slots to zero. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        tmp[i] = (seq_t)0;

    cache     = tmp;
    cache_len = n;
}

/* Ensure that all values up to 'n' have been computed.  */
static void precompute(idx_t n)
{
    if (cache_len >= n)
        return;

    idx_t old_len = cache_len;  /* previous maximum stored (may be 0) */
    grow_cache(n);

    for (idx_t i = old_len + 1; i <= n; ++i)
    {
        if (i <= 4)          /* use the initial values directly          */
            cache[i] = init_vals[i];
        else
            cache[i] = C19   * cache[i - 1]
                     + C304  * cache[i - 3]
                     + C256  * cache[i - 4];
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)              /* sequence is only defined for n ≥ 1 */
        return (seq_t)0;
    precompute(n);
    return cache[n];
}
