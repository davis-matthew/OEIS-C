/*
 * A317996  Expansion of e.g.f. exp((1 - exp(-3*x))/3).
 * https://oeis.org/A317996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-3)^(k-1) * binomial(n-1, k-1) * a(n-k)
 *            a(0) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming with memoisation.
   The array 'cache' stores already computed values of a(n).  Each
   new value is calculated by the recurrence above.  The array is
   resized as necessary.  The code is valid both in C11 and C++17.
*/

#include <stdlib.h>

#ifdef __cplusplus
// When compiled as C++: use 'new' and 'delete' to construct objects.
static seq_t *cache = nullptr;      // pointer to allocated array
static idx_t cache_size = 0;        // number of values stored, highest index + 1
static size_t cache_cap = 0;        // actual capacity of the array

static void ensure_capacity(idx_t n) {
    if (cache_cap > (size_t)n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
    cache = new_cache;
    cache_cap = new_cap;
}
#else
// When compiled as C: use malloc and free.  seq_t is assumed to be
// a trivially constructible type (e.g. builtin integer or __int128).
static seq_t *cache = NULL;        // pointer to allocated array
static idx_t cache_size = 0;       // number of values stored, highest index + 1
static size_t cache_cap = 0;       // actual capacity of the array

static void ensure_capacity(idx_t n) {
    if (cache_cap > (size_t)n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    // Initialise a(0)
    if (cache_size == 0) {
        ensure_capacity(0);
        cache[0] = (seq_t)1;          // a(0) = 1
        cache_size = 1;
    }

    if (n < (idx_t)cache_size)
        return cache[n];

    // Compute values up to n
    ensure_capacity(n);
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        seq_t power = (seq_t)1;      // (-3)^0
        seq_t c = (seq_t)1;          // C(i-1,0)

        for (idx_t k = 1; k <= i; ++k) {
            // sum += (-3)^(k-1) * C(i-1, k-1) * a(i-k)
            sum += power * c * cache[i - k];

            if (k < i) {
                // update power to (-3)^k
                power *= (seq_t)-3;

                // update binomial coefficient
                seq_t numerator = (seq_t)(i - 1) - (seq_t)(k - 1); // (i-1)-(k-1)
                c = (c * numerator) / (seq_t)k;
            }
        }

        cache[i] = sum;
    }
    cache_size = n + 1;
    return cache[n];
}
