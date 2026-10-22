/*
 * A397521  Expansion of ordinary generating function A(x) satisfying
 *          (A(x)^2-A(x^2))/2 = x/(1-x)^2.
 * https://oeis.org/A397521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 39, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  Sum_{k=0..n} a(k)*a(n-k) = 2*n + [n even] a(n/2)
   a(0) = 1
   Computes a(n) for n ≥ 0, memoizing results in a static array of size 10001.
   This implementation assumes that the defined index range is sufficient for the tests
   (up to about n = 10000).  */
#include <stddef.h>

#define MAX_CACHE 10001

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    /* lazily initialize base case */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    if (target < cache_len)
        return cache[target];

    /* grow the cache up to the requested index */
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k < i; ++k)
            sum += cache[k] * cache[i - k];
        seq_t numerator = (seq_t)i * 2;
        if ((i & 1) == 0)
            numerator += cache[i / 2];
        cache[i] = (numerator - sum) / 2;
    }
    cache_len = target + 1;
    return cache[target];
}
