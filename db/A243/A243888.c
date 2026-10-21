/*
 * A243888  Primes of the form 2*n^2+26*n+11.
 * https://oeis.org/A243888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 26*n + 11 for that n where the value is prime
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates 2*n²+26*n+11 and keeps all primes found in a static
   cache, so repeated calls for arbitrary n <=> O(n) total operations. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Helper to cast the result of realloc/malloc in a way that compiles both C
   and C++.  In C the cast is implicit; in C++ we must use reinterpret_cast. */
#if defined(__cplusplus)
# define ALLOC_CAST(ptr, type) reinterpret_cast<type>(ptr)
#else
# define ALLOC_CAST(ptr, type) ((type)(ptr))
#endif

seq_t a(idx_t n)
{
    /* static cache keeps primes that have already been found */
    static seq_t *cache      = NULL; /* array of found primes       */
    static size_t  capacity  = 0;    /* current allocated capacity  */
    static size_t  count     = 0;    /* number of stored primes     */
    static idx_t   last_test = 0;    /* last n that was tested      */

    /* generate primes until we have at least n of them */
    while (count < n) {
        /* test next n value */
        ++last_test;

        /* compute 2*n^2 + 26*n + 11 in seq_t arithmetic */
        seq_t sq   = (seq_t)last_test;          /* n                  */
        seq_t val  = sq * sq;                   /* n^2                */
        val        = val * (seq_t)2;            /* 2*n^2              */
        val        = val + (sq * (seq_t)26);    /* +26*n              */
        val        = val + (seq_t)11;           /* +11                */

        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = ALLOC_CAST(realloc(cache, newcap * sizeof(seq_t)), seq_t*);
                if (!tmp) {
                    /* allocation failed – best we can do is abort */
                    exit(EXIT_FAILURE);
                }
                cache    = tmp;
                capacity = newcap;
            }
            cache[count++] = val;
        }
    }

    return cache[n - 1];
}
