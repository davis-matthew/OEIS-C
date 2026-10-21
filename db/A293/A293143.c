/*
 * A293143  Number of vertex points in a Sierpinski carpet grid subdivided
 *          into squares: a(n+1) = 8*a(n) - 8*(3^n+1), a(0) = 4.
 * https://oeis.org/A293143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A293143
 * Recurrence: a(0) = 4,
 *              a(n+1) = 8 * a(n) - 8 * (3^n + 1)
 *
 * The implementation memoises computed terms and eagerly
 * calculates intermediate values up to the requested index.
 */

#include <stdlib.h>

/* external integer types provided by the build system */
extern seq_t a(idx_t n);          /* the function defined below */

/* internal memoisation state */
static seq_t *cache      = NULL;   /* array of cached a(n) values   */
static idx_t  capacity   = 0;      /* current size of cache array   */
static idx_t  last_idx   = 0;      /* largest index already stored  */
static seq_t  pow3       = 1;      /* 3^last_idx                    */

/* ensure capacity for indices up to `need` */
static void ensure_capacity(idx_t need)
{
    if (need < capacity)                /* already enough space     */
        return;

    /* grow until capacity > need          */
    idx_t new_cap = capacity ? capacity << 1 : 1;
    while (new_cap <= need)
        new_cap <<= 1;

    /* allocate new storage                         */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                        /* fatal error if OOM       */

    /* copy old cached values, if any              */
    if (cache) {
        for (idx_t i = 0; i <= last_idx; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache    = new_cache;
    capacity = new_cap;

    /* initialise first value when the cache is new   */
    if (last_idx == 0 && cache[0] == 0) { /* first initialisation */
        cache[0] = (seq_t)4;            /* a(0) = 4                */
        pow3     = (seq_t)1;            /* 3^0 = 1                 */
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;                 /* base case, no memoisation needed */

    /* make sure we have room for all needed terms */
    ensure_capacity(n);

    /* compute terms up to n if they are not yet cached */
    for (idx_t i = last_idx + 1; i <= n; ++i)
    {
        /* a(i) = 8*a(i-1) - 8*(pow3 + 1)   where pow3 = 3^(i-1) */
        seq_t term = (seq_t)8 * cache[i - 1] - (seq_t)8 * (pow3 + (seq_t)1);
        cache[i] = term;

        /* update pow3 to 3^i for the next iteration            */
        pow3 *= (seq_t)3;
    }

    last_idx = n;
    return cache[n];
}
