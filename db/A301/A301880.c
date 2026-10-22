/*
 * A301880  Number of n X 4 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence * a(n) = a(n-1) + 12*a(n-2) - 2*a(n-3) - 38*a(n-4) +
 *            a(n-5) + 12*a(n-6) - a(n-8) * requires a(1)=5, a(2)=18, a(3)=58,
 *            a(4)=203, a(5)=673, a(6)=2340, * a(7)=7883, a(8)=27175
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array of computed terms, index 0 unused   */
static size_t cache_cap = 0;          /* allocated capacity (>= cache size)       */
static idx_t  computed = 0;           /* highest n for which cache[n] is valid    */

static const seq_t base[9] = {
    (seq_t)0,                      /* dummy for index 0                         */
    (seq_t)5, (seq_t)18, (seq_t)58, (seq_t)203,
    (seq_t)673, (seq_t)2340, (seq_t)7883, (seq_t)27175
};

/* Grow the cache to accommodate indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;          /* we need indices 0..n           */
    if (new_cap <= cache_cap)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failed              */
    /* zero the newly allocated tail              */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute terms a(1)..a(n) as needed, using memoization. */
static void compute_up_to(idx_t n)
{
    if (computed >= n) return;              /* nothing to do on the cache */

    ensure_cache(n);

    /* Initialise the first eight terms on the first run */
    if (computed < 8) {
        for (idx_t i = 1; i <= 8 && i <= n; ++i)
            cache[i] = base[i];
        computed = (n < 8) ? n : 8;
    }

    /* Continue computing from the next missing index */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a8 = cache[i - 8];

        seq_t val = a1
                    + (seq_t)12 * a2
                    + a5
                    + (seq_t)12 * a6
                    - (seq_t)2 * a3
                    - (seq_t)38 * a4
                    - a8;

        cache[i] = val;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out-of-range; not used */
    compute_up_to(n);
    return cache[n];
}
