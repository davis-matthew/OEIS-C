/*
 * A259653  a(0)=0, a(1)=1, a(n) = min{3 a(k) + (3^(n-k)-1)/2, k=0..(n-1)} for
 *          n>=2.
 * https://oeis.org/A259653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=min{3*a(k)+(3^(n-k)-1)/2, k=0..n-1}.
 *            Derivation: Let e(n) be the exponent such that
 *            a(n)-a(n-1)=3^e(n). The exponents e(n) follow the rule
 *            e(n)=ceil((sqrt(8*n+1)-1)/2)-1. Hence a(n)=a(n-1)+3^e(n). This
 *            implementation uses that equivalent linear recurrence
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 173, u64 n <= 725,
 *            u128 n <= 3026, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache       = NULL;   /* array of already computed a(n) */
static size_t cache_cap   = 0;       /* capacity of cache array   */
static idx_t   computed   = 0;       /* last index (>=0) computed */
static idx_t   _k         = 1;       /* smallest k with current index <= k*(k+1)/2  */
static seq_t   pow3       = 1;       /* 3^(_k-1) */
static bool    initialized = false; /* one‑time initialisation flag */

/* enlarge the cache array so that it can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* external linkage function asked for */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!initialized) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache[0] = 0;                /* a(0)=0 */
        computed = 0;                /* we already have a(0) */
        _k = 1;                      /* e(0)=0; 3^0 = 1 stored in pow3 */
        pow3 = 1;
        initialized = true;
    }

    if (n == 0)
        return cache[0];

    /* if we have already computed up to n, just return the cached value */
    if (n <= computed)
        return cache[n];

    /* compute values up to n, extending cache as needed */
    ensure_capacity(n);

    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        /* advance _k while idx > T(_k) = _k*(_k+1)/2 */
        while (idx > _k * (_k + 1) / 2) {
            ++_k;
            pow3 = pow3 * 3;          /* pow3 now equals 3^(_k-1) */
        }
        cache[idx] = cache[idx - 1] + pow3;
        computed = idx;
    }
    return cache[n];
}
