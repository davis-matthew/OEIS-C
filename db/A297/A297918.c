/*
 * A297918  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) + 6*a(n-3) -17*a(n-4) -12*a(n-5)
 *            -7*a(n-6) -36*a(n-7) + 6*a(n-8) -19*a(n-9) for n > 12 Initial
 *            values: a(1)=1 a(2)=17 a(3)=109 a(4)=588 a(5)=3276 a(6)=18451
 *            a(7)=103558 a(8)=581318 a(9)=3263838 a(10)=18323588
 *            a(11)=102871721 a(12)=577541657
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;        /* 1‑based cache, cache[0] unused */
static size_t cap = 0;             /* current allocation in slots */
static idx_t max_computed = 0;     /* highest index computed so far */

/* Ensure the cache can store at least `min_cap` slots (index ≥ min_cap-1). */
static void ensure_cache(size_t min_cap)
{
    if (cap >= min_cap)
        return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate */
        abort();
    }
    cache = new_cache;
    cap = new_cap;
}

/* Initialise cache with known terms a(1) … a(12). */
static void init_cache(void)
{
    /* initial terms, index 0 is unused */
    static const seq_t init_vals[13] = {
        0ULL,          /* placeholder for index 0 */
        1ULL, 17ULL, 109ULL, 588ULL, 3276ULL,
        18451ULL, 103558ULL, 581318ULL, 3263838ULL,
        18323588ULL, 102871721ULL, 577541657ULL
    };
    ensure_cache(13);                /* make space for indices 0 … 12 */
    for (idx_t i = 1; i <= 12; ++i)
        cache[i] = init_vals[i];
    max_computed = 12;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* outside the defined range – safe fall‑through */

    /* allocate cache and initialise if first call */
    if (!cache)
        init_cache();
    else if (max_computed < 12)
        init_cache();         /* may happen if a call with a small n occurs first */

    /* extend cache to hold the requested index */
    ensure_cache((size_t)n + 1);   /* cache indices 0 … n  (n+1 slots) */

    if (max_computed >= n)      /* already computed */
        return cache[n];

    /* Compute missing terms using the recurrence.  The recurrence
       is valid for i > 12; for i <= 12 the cached values are used. */
    idx_t start = (max_computed < 13) ? 13 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t v =  5 * cache[i-1]
                 + 3 * cache[i-2]
                 + 6 * cache[i-3]
                 -17 * cache[i-4]
                 -12 * cache[i-5]
                 - 7 * cache[i-6]
                 -36 * cache[i-7]
                  +6 * cache[i-8]
                 -19 * cache[i-9];
        cache[i] = v;
    }
    max_computed = n;
    return cache[n];
}
