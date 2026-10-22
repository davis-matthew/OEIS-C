/*
 * A363707  For n >= 2, a(n) = a(n-1) + a(n-2) + A051697(n), a(0)=0, a(1)=1.
 * https://oeis.org/A363707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + p(n)
 * where p(n) is the nearest prime to n (if n is equally distant from two
 * primes, the smaller prime is taken).  Initial values: a(0)=0, a(1)=1.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t nearest_prime(idx_t n)
{
    if (n < 2)                  /* n should never be <2 for this sequence */
        return (seq_t)2;
    if (is_prime(n))
        return (seq_t)n;
    for (idx_t d = 1; ; ++d) {
        if (n >= d && is_prime(n - d))
            return (seq_t)(n - d);
        if (is_prime(n + d))
            return (seq_t)(n + d);
    }
}

static seq_t *a_cache       = NULL;
static size_t a_cache_cap   = 0;
static idx_t  max_computed  = 0;   /* highest index that has been computed */

static void ensure_cached(idx_t n)
{
    if (n <= max_computed)
        return;

    if (a_cache == NULL) {
        a_cache_cap = 2;           /* store a(0) and a(1) */
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n + 1 > a_cache_cap) {
        size_t new_cap = a_cache_cap * 2;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t pk = nearest_prime(k);
        a_cache[k] = a_cache[k - 1] + a_cache[k - 2] + pk;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    ensure_cached(n);
    return a_cache[(size_t)n];
}
