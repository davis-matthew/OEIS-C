/*
 * A379993  Expansion of e.g.f. 1/(1 - x * exp(x))^4.
 * https://oeis.org/A379993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(k+3,3)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static seq_t *fact_arr = NULL;   /* factorials 0.. */
static size_t fact_cap = 0;

static void ensure_fact(idx_t n)
{
    if (fact_cap > n) return;
    size_t old = fact_cap;
    size_t new_cap = old ? old << 1 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    fact_arr = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    if (old == 0) {            /* initialise 0! */
        fact_arr[0] = 1;
        old = 1;
    }
    for (size_t i = old; i <= (size_t)n; ++i)
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;
    fact_cap = new_cap;
}

static seq_t *cache_arr = NULL;   /* memoised a(n) */
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t old = cache_cap;
    size_t new_cap = old ? old << 1 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache_arr = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
    for (size_t i = old; i <= (size_t)n; ++i)
        cache_arr[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache_arr[n] != 0) return cache_arr[n];

    ensure_fact(n);

    seq_t sum = 0;
    seq_t pk   = 1;          /* P(n,0) = n!/(n-0)! = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t pow_val;
        if (k == 0) {
            pow_val = (n == 0) ? 1 : 0;   /* 0^0 = 1, otherwise 0 */
        } else {
            pow_val = big_pow((seq_t)k, (seq_t)(n - k));
        }

        seq_t b   = binomial((idx_t)(k + 3), (idx_t)3);
        seq_t term = pk * pow_val * b;
        sum += term;

        if (k < n)
            pk *= (seq_t)(n - k);          /* next P(n,k+1) */
    }

    cache_arr[n] = sum;
    return sum;
}
