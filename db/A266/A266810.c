/*
 * A266810  Decimal representation of the n-th iteration of the "Rule 62"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies * a(0)=1, a(1)=7, a(2)=25, a(3)=111, a(4)=433, *
 *            a(5)=1751, a(6)=7033, a(7)=28047, * a(n) = 4*a(n-1) + 8*a(n-3) -
 *            31*a(n-4) - 4*a(n-5) * - 8*a(n-7) + 32*a(n-8) for n ≥ 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache that remembers terms already computed */
static seq_t *cache = NULL;          /* array of computed values   */
static size_t  cache_cap = 0;        /* allocated capacity         */
static idx_t   cache_len = 0;        /* number of valid entries    */

/* initialise cache with the first 8 terms */
static void init_cache(void)
{
    if (cache_len != 0) return;          /* already initialised */

    size_t init_cap = 8;
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!cache) exit(1);                 /* allocation failed */

    cache[0] = 1;
    cache[1] = 7;
    cache[2] = 25;
    cache[3] = 111;
    cache[4] = 433;
    cache[5] = 1751;
    cache[6] = 7033;
    cache[7] = 28047;

    cache_cap = init_cap;
    cache_len = 8;
}

/* enlarge the cache to at least size n+1, and compute
 * any missing terms up to index n */
static void ensure_cache(idx_t n)
{
    init_cache();                        /* make sure base terms exist */

    if (n < cache_len) return;           /* already computed */

    /* grow capacity: at least double until it fits n+1 */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* term  = 4*a(i-1) + 8*a(i-3) + 32*a(i-8) */
        seq_t term = cache[i-1];
        term = term * 4 + cache[i-3] * 8 + cache[i-8] * 32;

        /* sub   = 31*a(i-4) + 4*a(i-5) + 8*a(i-7) */
        seq_t sub  = cache[i-4] * 31 + cache[i-5] * 4 + cache[i-7] * 8;

        cache[i] = term - sub;            /* safe: term ≥ sub → positive result */
    }
    cache_len = n + 1;
}

/* public function requested by the harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
