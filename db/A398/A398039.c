/*
 * A398039  G.f. A(x) satisfies A(x) = A(x^2) / (1-3*x), with A(0) = 1.
 * https://oeis.org/A398039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3*a(n-1) if n odd; a(n) = 3*a(n-1) + a(n/2) if
 *            n even. The function memoizes results in a static array. It is
 *            safe for any seq_t type that supports +, -, *, /, and the cast
 *            (seq_t)3. The array is grown with realloc, cast to (seq_t*)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t  a_cache_cap = 0;   /* current allocated capacity */
static idx_t   a_cache_len  = 0;   /* number of values stored */

seq_t a(idx_t n)
{
    /* Ensure enough capacity in the cache. */
    if (n + 1 > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Initialise the first element if needed. */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;
        a_cache_len = 1;
    }

    /* If the requested value is already computed, return it. */
    if (n < a_cache_len)
        return a_cache[n];

    /* Compute all values up to n inclusively. */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if ((i & 1) != 0) {                     /* odd */
            a_cache[i] = a_cache[i-1] * (seq_t)3;
        } else {                                 /* even */
            a_cache[i] = a_cache[i-1] * (seq_t)3 + a_cache[i/2];
        }
    }
    a_cache_len = n + 1;
    return a_cache[n];
}
