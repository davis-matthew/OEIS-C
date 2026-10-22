/*
 * A380652  Shifts left one place under the inverse modulo 2 binomial
 *          transform.
 * https://oeis.org/A380652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^A010060(k) * (binomial(n-1,k) mod 2)
 *            * a(n-k-1) Initial value: a(0) = 1 A010060(k) = number of 1‑bits
 *            in k (mod 2) → (-1)^A010060(k) is +1 if popcount(k) is even, and
 *            -1 if popcount(k) is odd. The binomial coefficient is odd iff (k
 *            & ((n-1)-k)) == 0 (Lucas' theorem)
 * Range:     i8 n <= 39, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache      = NULL;   /* array of computed a(n) values */
static idx_t cache_cap   = 0;      /* current capacity of cache   */
static idx_t cache_len   = 0;      /* number of valid elements in cache */

/* ensure cache has capacity for at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                   /* out of memory */
    cache     = tmp;
    cache_cap = (idx_t)new_cap;
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* initialise base case */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_cap)
        grow_cache(n + 1);

    /* compute missing values in increasing order */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t prev = i - 1;                /* n-1 for this step */
        __int128 sum128 = 0;               /* 128‑bit accumulator to avoid overflow */

        for (idx_t k = 0; k < i; ++k) {
            /* binomial(prev,k) is odd iff (k & (prev-k)) == 0  (Lucas' theorem) */
            if ((k & (prev - k)) == 0) {
                bool parity = __builtin_parityll((unsigned long long)k); /* popcount(k) mod 2 */
                seq_t   sign   = parity ? (seq_t)-1 : (seq_t)+1;
                sum128 += (__int128)sign * (__int128)cache[i - k - 1];
            }
        }
        cache[i] = (seq_t)sum128;          /* cast back to seq_t (int64_t or big integer) */
    }
    cache_len = n + 1;
    return cache[n];
}
