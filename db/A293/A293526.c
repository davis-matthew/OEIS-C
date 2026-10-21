/*
 * A293526  Expansion of e.g.f. exp(x^3/(1 - x^4)).
 * https://oeis.org/A293526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the 8‑term recurrence * * a(n+8) =
 *            n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7)*a(n) * -
 *            (n+7)*(n+6)*(n+5)*(n+4)*(n+3)*(n+2)*a(n+1) * -
 *            2*(n+5)*(n+4)*(n+7)*(n+6)*a(n+4) * - 3*(n+7)*(n+6)*a(n+5) * *
 *            Initial values: * a(0)=1, a(1)=0, a(2)=0, a(3)=6, a(4)=0,
 *            a(5)=0, * a(6)=360, a(7)=5040
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static memoization cache */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;

/* Ensure cache has at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_capacity)
        return;
    size_t new_cap = need * 2;
    if (new_cap < 16)
        new_cap = 16;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
    cache_capacity = new_cap;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* allocation failure */
    }
    cache = tmp;
    cache_capacity = new_cap;
#endif
}

/* Compute all terms up to index n and store in cache */
static void compute_up_to(idx_t n)
{
    if (cache_len > 0 && cache_len - 1 >= n)
        return;

    if (cache_len == 0) {
        ensure_cache_capacity(8);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)360;
        cache[7] = (seq_t)5040;
        cache_len = 8;
    }

    for (size_t i = cache_len; i <= n; ++i) {
        /* Corresponding recurrence index k = i-8 */
        idx_t k = (idx_t)(i - 8);

        /* p1 = k*(k+1)*...*(k+7) */
        seq_t p1 = (seq_t)k;
        for (idx_t t = 1; t <= 7; ++t)
            p1 *= (seq_t)(k + t);

        /* p2 = (k+7)*(k+6)*...*(k+2) */
        seq_t p2 = (seq_t)(k + 7);
        p2 *= (seq_t)(k + 6);
        p2 *= (seq_t)(k + 5);
        p2 *= (seq_t)(k + 4);
        p2 *= (seq_t)(k + 3);
        p2 *= (seq_t)(k + 2);

        /* p3 = 2*(k+5)*(k+4)*(k+7)*(k+6) */
        seq_t p3 = (seq_t)(k + 5);
        p3 *= (seq_t)(k + 4);
        p3 *= (seq_t)(k + 7);
        p3 *= (seq_t)(k + 6);
        p3 = (seq_t)2 * p3;

        /* p4 = 3*(k+7)*(k+6) */
        seq_t p4 = (seq_t)(k + 7);
        p4 *= (seq_t)(k + 6);
        p4 = (seq_t)3 * p4;

        /* Compute terms */
        seq_t term1 = p1 * cache[k];
        seq_t term2 = p2 * cache[k + 1];
        seq_t term3 = p3 * cache[k + 4];
        seq_t term4 = p4 * cache[k + 5];

        /* a(k+8) = term2 + term3 + term4 - term1 */
        seq_t val = term2 + term3 + term4 - term1;

        ensure_cache_capacity(i + 1);
        cache[i] = val;
        ++cache_len;
    }
}

/* Public function: a(idx_t n) */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
