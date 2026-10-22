/*
 * A303073  L.g.f.: log(1 + Sum_{k>=1} prime(k)*x^k) = Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A303073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*prime(n) - sum_{k=1}^{n-1} a(k)*prime(n-k) (OEIS
 *            A303073)
 * Range:     i8 n <= 12, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *a_cache = NULL;   /* cache of a[0..] */
static idx_t a_cache_size = 0;  /* highest index computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Allocate or grow cache up to index n */
    if (a_cache == NULL || a_cache_size < n) {
        size_t new_size = (size_t)(n + 1);              /* allow direct access by index */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        a_cache = tmp;

        /* Compute missing values */
        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                sum += a_cache[k] * (seq_t)get_prime(i - k);
            }
            a_cache[i] = (seq_t)i * (seq_t)get_prime(i) - sum;
        }
        a_cache_size = n;
    }

    return a_cache[n];
}
