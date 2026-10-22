/*
 * A398186  a(n) = (1/n) * Sum_{k=0..n-1} A000897(k) * A000897(n-1-k), where
 *          A000897(k) = (4*k)!/((2*k)!*k!^2).
 * https://oeis.org/A398186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} A000897(k) * A000897(n-1-k) *
 *            (A000897(k) = (4*k)! / ((2*k)! * k!^2)) * Recurrence (valid for
 *            n >= 2): * n^3 (n+1) a(n+1) = 8 n (2n-1) (8n^2-8n+3) a(n) * -
 *            1024 (n-1)^2 (2n-3) (2n-1) a(n-1) * Initial values: a(1)=1,
 *            a(2)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* forward declaration of the function required by the tests */
seq_t a(idx_t n);

static seq_t *cache      = NULL;   /* array of computed a(i), 1‑based */
static size_t cache_cap = 0;      /* allocated size of cache array   */
static idx_t  cache_tail = 0;      /* largest index that has a value */

/* ensure cache array can hold indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = cache_cap * 2;
    if (new_cap <= n)
        new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failure: caller will see zero value */

    if (cache)
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) using the recurrence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not part of the OEIS offset */

    if (cache_tail == 0) {
        /* first time: allocate cache for indices 1 and 2 */
        ensure_cache(2);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)12;
        cache_tail = 2;
    }

    if (n <= cache_tail)
        return cache[n];

    ensure_cache(n);

#if defined(__SIZEOF_INT128__)
    /* use 128‑bit intermediate when seq_t is a small integer type */
    for (idx_t i = cache_tail; i < n; ++i) {
        __uint128_t ni   = (__uint128_t)i;
        __uint128_t a_i  = (__uint128_t)cache[i];
        __uint128_t a_im = (__uint128_t)cache[i-1];

        /* term1 = 8*n*(2n-1)*(8n^2-8n+3)*a(n) */
        __uint128_t term1 = (__uint128_t)8 * ni * (2*ni - 1) *
                            (8*ni*ni - 8*ni + 3) * a_i;

        /* term2 = 1024*(n-1)^2*(2n-3)*(2n-1)*a(n-1) */
        __uint128_t term2 = (__uint128_t)1024 * (ni-1) * (ni-1) *
                            (2*ni - 3) * (2*ni - 1) * a_im;

        __uint128_t numer = term1 - term2;
        __uint128_t denom = ni*ni*ni*(ni+1);

        __uint128_t res = numer / denom;
        cache[i+1] = (seq_t)res;
    }
#else
    /* fallback: use big‑integer arithmetic only (will overflow for 64‑bit) */
    for (idx_t i = cache_tail; i < n; ++i) {
        seq_t ai  = cache[i];
        seq_t aim = cache[i-1];

        seq_t term1 = ai * (seq_t)8;
        term1 = term1 * (seq_t)i;
        term1 = term1 * (seq_t)(2*i - 1);
        term1 = term1 * (seq_t)(8*i*i - 8*i + 3);

        seq_t term2 = aim * (seq_t)1024;
        term2 = term2 * (seq_t)(i-1);
        term2 = term2 * (seq_t)(i-1);
        term2 = term2 * (seq_t)(2*i - 3);
        term2 = term2 * (seq_t)(2*i - 1);

        seq_t numer = term1 - term2;
        seq_t denom = (seq_t)i * (seq_t)i * (seq_t)i * (seq_t)(i+1);
        seq_t res = numer / denom;
        cache[i+1] = res;
    }
#endif

    cache_tail = n;
    return cache[n];
}
