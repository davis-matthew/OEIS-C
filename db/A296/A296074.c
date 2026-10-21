/*
 * A296074  Sum of deficiencies of the proper divisors of n.
 * https://oeis.org/A296074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n, d<n} (2*d - sigma(d))
 * Range:     i8 n <= 215, i16 n <= 9625, i32 n <= 9625, i64 n <= 9625,
 *            i128 n <= 9625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033879(d) is the deficiency of d, i.e. 2*d - sigma(d). */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for divisor sums sigma(d) */
static seq_t *sigma_cache = NULL;      /* sigma_cache[0] unused */
static idx_t  sigma_cache_size = 0;    /* maximum index computed */

/* Ensure sigma_cache has entries up to n */
static void ensure_sigma(idx_t n)
{
    if (n <= sigma_cache_size)
        return;

    idx_t old_size = sigma_cache_size;
    idx_t new_size = n;

    /* Reallocate cache to new size (+1 because we use 1-based indices) */
    seq_t *new_cache = (seq_t *)realloc(sigma_cache,
                                        (new_size + 1) * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failed */

    /* Initialize new entries to 0; old entries already contain correct values. */
    for (idx_t i = old_size + 1; i <= new_size; ++i)
        new_cache[i] = (seq_t)0;

    /* Add contributions of all divisors to indices > old_size */
    for (idx_t i = 1; i <= new_size; ++i) {
        idx_t first = ((old_size + 1 + i - 1) / i) * i;   /* first multiple > old_size */
        for (idx_t j = first; j <= new_size; j += i) {
            new_cache[j] += (seq_t)i;   /* i is a divisor of j */
        }
    }

    sigma_cache = new_cache;
    sigma_cache_size = new_size;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;   /* a(1) = 0, no proper divisors */

    ensure_sigma(n);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 < n) {
                seq_t def1 = (seq_t)2 * (seq_t)d1 - sigma_cache[d1];
                sum += def1;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n) {
                seq_t def2 = (seq_t)2 * (seq_t)d2 - sigma_cache[d2];
                sum += def2;
            }
        }
    }
    return sum;
}
