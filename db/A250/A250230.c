/*
 * A250230  Number of length 3+1 0..n arrays with the sum of the cubes of
 *          adjacent differences multiplied by some arrangement of +-1 equal
 *          to zero.
 * https://oeis.org/A250230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) (n>=5) Initial values:
 *            a(1)=8, a(2)=27, a(3)=52, a(4)=89
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 145, u64 n <= 145,
 *            u128 n <= 145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of computed values, indices 0..n */
static idx_t max_cached = 0;         /* largest n that has been computed */
static int initialized = 0;

/* initialise cache with the first four terms */
static void init_cache(void)
{
    if (initialized) return;
    size_t sz = 5; /* indices 0..4, 0 unused */
    cache = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!cache) abort();
    for (idx_t i = 0; i < 5; ++i) cache[i] = 0;
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)27;
    cache[3] = (seq_t)52;
    cache[4] = (seq_t)89;
    max_cached = 4;
    initialized = 1;
}

/* extend the cache up to n using the recurrence */
static void extend_cache(idx_t n)
{
    if (n <= max_cached) return;
    size_t new_size = (size_t)(n + 1);          /* need indices 0..n */
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t t = cache[i-1] + cache[i-1];      /* 2*a(i-1) */
        seq_t s = cache[i-3] + cache[i-3];      /* 2*a(i-3) */
        t -= s;
        t += cache[i-4];
        cache[i] = t;
    }
    max_cached = n;
}

/* required external function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 */
    init_cache();
    extend_cache(n);
    return cache[n];
}
