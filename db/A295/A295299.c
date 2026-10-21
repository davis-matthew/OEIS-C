/*
 * A295299  Numbers k such that the sum of the divisors (A000203) and the
 *          binary weight of k (A000120) have different parity.
 * https://oeis.org/A295299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k for which the sum of divisors σ(k) *
 *            and the binary weight popcount(k) have different parity. *
 *            Recall σ(k) is odd iff k is a square or twice a square
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Integer square root using Newton's method.  Returns floor(sqrt(n)). */
static idx_t isqrt_idx_t(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x >> 1) + 1;   /* initial guess */
    while (y < x)
    {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* Test whether n is a perfect square. */
static bool is_square(idx_t n)
{
    idx_t r = isqrt_idx_t(n);
    __int128 rr = (__int128)r * r;
    return rr == n;
}

/* Test whether n is twice a perfect square. */
static bool is_twice_square(idx_t n)
{
    if (n & 1ULL) return false;   /* odd */
    idx_t half = n >> 1;          /* n / 2 */
    return is_square(half);
}

/* Global cache for memoisation. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_k = 0;      /* last k examined */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        ++last_k;   /* next integer to test */

        bool sigma_odd = is_square(last_k) || is_twice_square(last_k);
        bool popp_odd  = (__builtin_popcountll((unsigned long long)last_k) & 1ULL);

        if (sigma_odd != popp_odd)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
