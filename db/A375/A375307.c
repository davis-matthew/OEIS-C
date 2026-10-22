/*
 * A375307  a(n) = Sum_{k=0..floor(3*n/5)} binomial(3*n-3*k,2*k).
 * https://oeis.org/A375307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375307
 *
 *  Formula: a(n) = Σ_{k=0..⌊3n/5⌋} binomial(3n-3k, 2k)
 *  a(n) = 2*a(n-1) + 5*a(n-2) + 3*a(n-3) - 3*a(n-4) + a(n-5)
 *  initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=16, a(4)=52
 *
 *  The sequence is computed by memoising the recurrence.  The routine
 *  works for any seq_t (unsigned integer or big‑integer class) and any
 *  idx_t (uint64_t).  It is fully compliant with C11 and C++17. */

#include <stdlib.h>

/* Cache for computed terms */
static seq_t *cache = NULL;      /* array holding a(0) … a(n)           */
static size_t cache_cap = 0;     /* allocated size in terms              */
static size_t len = 0;           /* number of terms already computed     */

/* Ensure the cache can hold at least (n+1) terms and initialise
 * the first five terms on the very first call. */
static void ensure_cache(idx_t n)
{
    /* First allocation and initialisation */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1 > 16 ? n + 1 : 16);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();     /* out of memory */

        /* initial values of the recurrence */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)52;
        len = 5;                 /* indices 0 … 4 are valid */
    }

    /* Grow the array if necessary */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap << 1;          /* double */
        if (new_cap <= cache_cap || new_cap > n + 1)
            new_cap = (size_t)n + 1;             /* safety for overflow */

        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();                         /* out of memory */
        for (size_t i = 0; i < len; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n).  The routine is memoised and works for any seq_t,
 * including arbitrary‑precision big integers. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If already computed, just return it */
    if (n < (idx_t)len)
        return cache[(size_t)n];

    /* Compute new terms using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)2 * cache[i-1]
                  + (seq_t)5 * cache[i-2]
                  + (seq_t)3 * cache[i-3]
                  + (seq_t)1 * cache[i-5];
        cache[i] = sum - (seq_t)3 * cache[i-4];
    }
    len = (size_t)n + 1;   /* update cached length */
    return cache[(size_t)n];
}
