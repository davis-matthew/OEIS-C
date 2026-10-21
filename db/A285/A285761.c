/*
 * A285761  A slow relative of Hofstadter's Q sequence.
 * https://oeis.org/A285761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 527, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-4-a(n-1)) + a(n-4-a(n-4)),
   a(1)=1, a(2)=2, a(3)=a(4)=a(5)=3,
   a(6)=a(7)=a(8)=4, a(9)=a(10)=5. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for memoised values */
static seq_t *cache   = NULL;   /* cache[0] unused */
static size_t cap     = 0;      /* allocated size (in elements) */
static idx_t  computed = 0;     /* highest index computed so far */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    /* Need space for entries 0 .. n (n+1 elements). */
    if ((size_t)(n) + 1 <= cap)
        return;               /* already big enough */

    size_t newcap = cap ? cap : 16;
    while (newcap < (size_t)n + 1)
        newcap <<= 1;          /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();        /* out of memory */
    cache = tmp;
    cap   = newcap;
}

/* Initialise the cache with the known first 10 terms. */
static void
init_cache(void)
{
    if (computed > 0)
        return;                /* already initialised */

    ensure_cache(10);

    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 3;
    cache[4] = 3;
    cache[5] = 3;
    cache[6] = 4;
    cache[7] = 4;
    cache[8] = 4;
    cache[9] = 5;
    cache[10] = 5;

    computed = 10;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n=0, safe return */

    init_cache();             /* ensure the base terms are available */

    if (n > computed) {
        ensure_cache(n);      /* make room for new indices */

        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Compute the two indices using signed arithmetic
               to avoid unsigned underflow.  For n up to 10000
               the values safely fit in a signed 64‑bit integer. */
            long long t1 = (long long)i - 4 - (long long)cache[i - 1];
            long long t2 = (long long)i - 4 - (long long)cache[i - 4];
            idx_t j1 = (idx_t)t1;
            idx_t j2 = (idx_t)t2;

            /* The recurrence guarantees that t1 and t2 are >= 1. */
            cache[i] = cache[j1] + cache[j2];
        }

        computed = n;
    }

    return cache[n];
}
