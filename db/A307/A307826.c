/*
 * A307826  The number of integers r such that all primes above a certain
 *          value have the form primorial(n)*q +- r.
 * https://oeis.org/A307826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A002110(n))/2 for n>1, a(1)=1 * (Equivalently
 *            a(n) = ∏_{k=1..n} (p_k-1) / 2, with p_k the k‑th prime)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; n=0 is outside the sequence */
        return (seq_t)0;

    /* static cache for memoised values a(1)..a(max_computed) */
    static seq_t *cache   = NULL;   /* array of cached a(k) */
    static size_t cache_size = 0;   /* current allocated size */
    static idx_t max_computed = 0;  /* largest n for which cache is valid */
    static seq_t prod = (seq_t)1;   /* ∏_{k=1..max_computed} (p_k-1) */

    /* Ensure we have room for a(n) */
    ensure_cache(n, &cache, &cache_size);

    /* Compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            /* a(1) is defined to be 1; product remains 1 */
            cache[0] = (seq_t)1;
            prod    = (seq_t)1;
        } else {
            uint64_t p = get_prime(i);         /* i‑th prime, 1‑indexed */
            prod = prod * (seq_t)(p - 1);      /* update product of (p_j-1) */
            cache[i - 1] = prod / (seq_t)2;    /* a(i) = prod / 2 */
        }
    }

    max_computed = n;
    return cache[n - 1];
}
