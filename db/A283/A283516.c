/*
 * A283516  Number of nX2 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -18*a(n-2) -24*a(n-3) +39*a(n-4) +75*a(n-5)
 *            +69*a(n-6) +93*a(n-7) +99*a(n-8) +67*a(n-9) +45*a(n-10)
 *            +33*a(n-11) +16*a(n-12) +6*a(n-13) +3*a(n-14) +a(n-15) Initial
 *            values: a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=28, a(6)=145,
 *            a(7)=703, a(8)=3288, a(9)=14980, a(10)=66958, a(11)=294864,...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

static const seq_t init_a[15] = {
    (seq_t)0,   /* a(1) */
    (seq_t)0,   /* a(2) */
    (seq_t)1,   /* a(3) */
    (seq_t)5,   /* a(4) */
    (seq_t)28,  /* a(5) */
    (seq_t)145, /* a(6) */
    (seq_t)703, /* a(7) */
    (seq_t)3288,/* a(8) */
    (seq_t)14980,/* a(9) */
    (seq_t)66958,/* a(10) */
    (seq_t)294864,/* a(11) */
    (seq_t)1283142,/* a(12) */
    (seq_t)5529698,/* a(13) */
    (seq_t)23637592,/* a(14) */
    (seq_t)100350059/* a(15) */
};

static seq_t *cache = NULL;   /* dynamically enlarged cache  */
static size_t cache_cap = 0;   /* allocated size of cache      */
static size_t cache_len = 0;   /* number of terms actually stored */

/* Allocate or reallocate cache to hold at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                           /* memory exhausted */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the first 15 terms into the cache (if not already done). */
static void init_cache(void)
{
    if (cache_len >= 15)
        return;
    for (size_t i = 0; i < 15; ++i)
        cache[i] = init_a[i];
    cache_len = 15;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* guard against 0‑based call */
        return (seq_t)0;

    if (n <= 15U)                    /* direct lookup for the first 15 */
        return init_a[(size_t)n - 1];

    /* Prepare cache to store all terms up to n. */
    ensure_cache(n);
    init_cache();                    /* guarantee that first 15 are set */

    size_t limit = (size_t)n;        /* use size_t for indexing */
    for (size_t i = cache_len; i < limit; ++i)
    {
        /* Positive contributions */
        seq_t pos = 0;
        pos += cache[i - 1] * 9u;
        pos += cache[i - 4] * 39u;
        pos += cache[i - 5] * 75u;
        pos += cache[i - 6] * 69u;
        pos += cache[i - 7] * 93u;
        pos += cache[i - 8] * 99u;
        pos += cache[i - 9] * 67u;
        pos += cache[i -10] * 45u;
        pos += cache[i -11] * 33u;
        pos += cache[i -12] * 16u;
        pos += cache[i -13] * 6u;
        pos += cache[i -14] * 3u;
        pos += cache[i -15];              /* coefficient 1 */

        /* Negative contributions */
        seq_t neg = 0;
        neg += cache[i - 2] * 18u;
        neg += cache[i - 3] * 24u;

        cache[i] = pos - neg;            /* recurrence */
    }

    cache_len = limit;
    return cache[limit - 1];
}
