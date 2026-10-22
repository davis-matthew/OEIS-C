/*
 * A374556  Number of maximal independent vertex sets in the n-Goldberg graph.
 * https://oeis.org/A374556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374556: number of maximal independent vertex sets in the n‑Goldberg graph.
   Recurrence order 20:
     a(n) = 4*a(n-1)+23*a(n-2)+113*a(n-3)+52*a(n-4)+385*a(n-5)
            -862*a(n-6)+518*a(n-7)+64*a(n-8)-651*a(n-9)-102*a(n-10)
            +79*a(n-11)+970*a(n-12)+300*a(n-13)-469*a(n-14)-343*a(n-15)
            -72*a(n-16)+46*a(n-17)+65*a(n-18)-8*a(n-19)-4*a(n-20)
   The first 20 terms (n=1…20) are provided to seed the recurrence. */

#include <stdlib.h>

static const int coeff[20] = {
    4,   23, 113,  52,  385,
   -862, 518,   64, -651, -102,
     79, 970, 300, -469, -343,
    -72,  46, 65,  -8,  -4
};

/* Initial values a(1) … a(20); a(0) is unused but set to 0 for safety */
static const seq_t init[21] = {
    0,
    4, 62, 679, 4802, 43964,
    362621, 3063638, 26023026, 219108064,
    1853148122, 15656192182, 132249149729,
    1117453853962, 9440565205028, 79760002749149,
    673865612643090, 5693211039917562, 48099847738288178,
    406377235046133496, 3433326098510195722
};

/* Cached values of the sequence; 1‑based indexing */
static seq_t *cache = NULL;
static size_t cache_cap = 0;        /* allocated size (indices 0 … cache_cap-1) */
static idx_t max_computed = 0;      /* greatest index whose value is valid */

/* Ensure that cache can hold up to index n (inclusive) and that initial
   terms a(1)…a(20) are present when possible. */
static void ensure_capacity(idx_t n) {
    if (n + 1 <= cache_cap)
        return;                      /* already large enough */

    size_t new_cap = n + 1;           /* indices 0 … n */
    if (new_cap < 64)
        new_cap = 64;                 /* start with a small size */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* out of memory */

    /* Set new slots to zero. This works for built‑in integers and
       arbitrary‑precision types. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;

    if (max_computed == 0) {
        /* Copy known initial values if they fit in the new array. */
        for (idx_t i = 1; i <= 20 && i <= n; ++i)
            cache[i] = init[i];
        max_computed = (n >= 20) ? 20 : (idx_t)n;
    }
}

/* Compute a(n) for n >= 1.  Works for arbitrary call order. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                     /* undefined for offset=1, but safe */

    if (n <= 20)
        return init[n];

    ensure_capacity(n);

    /* Compute missing values up to n in increasing order. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 20; ++j) {
            seq_t term = cache[i - (size_t)(j + 1)];
            int  c = coeff[j];
            if (c >= 0)
                sum += (seq_t)c * term;
            else
                sum -= (seq_t)(-c) * term;
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
