/*
 * A347638  Number of minimal dominating sets in the n-dipyramidal graph (for
 *          n > 3).
 * https://oeis.org/A347638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) – a(n-3) – a(n-5) + 2*a(n-7) – a(n-8); Initial
 *            values: a(1)=3, a(2)=7, a(3)=10, a(4)=15, a(5)=16, a(6)=18,
 *            a(7)=29, a(8)=31. The recurrence holds for all n ≥ 9
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* a static cache holds computed terms; indices start at 1. */
static seq_t *cache = NULL;      /* array of seq_t values */
static size_t cap    = 0;        /* allocated size of the cache */
static idx_t  last   = 0;        /* highest index that has been computed */

/* make sure the cache can store indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap << 1 : 16;   /* grow exponentially */
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* memory exhausted */
        cache = tmp;
        cap   = new_cap;
    }
}

/* initialise the cache with the known first eight terms */
static void init_cache(void)
{
    if (last > 0) return;              /* already initialised */

    /* we need room at least for indices 1..8 */
    ensure_capacity(8);

    cache[1] = (seq_t)3;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)15;
    cache[5] = (seq_t)16;
    cache[6] = (seq_t)18;
    cache[7] = (seq_t)29;
    cache[8] = (seq_t)31;
    last = 8;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    init_cache();                  /* guarantees cache[1..8] set */

    if ((size_t)n >= cap)          /* enlarge if the caller requests a huge n */
        ensure_capacity(n);

    if (n <= last)                 /* value already known */
        return cache[n];

    /* compute successive values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence uses terms that are already computed because i ≥ 9 */
        seq_t val = 2 * cache[i - 1];
        val -= cache[i - 3];
        val -= cache[i - 5];
        val += 2 * cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
