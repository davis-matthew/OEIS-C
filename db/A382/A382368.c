/*
 * A382368  Expansion of 1/( 1 - 4 * Sum_{k>=0} x^(3^k) / (1 - x^(3^k))
 *          )^(1/2).
 * https://oeis.org/A382368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   v3_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A382368:  Expansion of 1/( 1 - 4 * Sum_{k>=0} x^(3^k) / (1 - x^(3^k)) )^(1/2).
    The series satisfies the functional equation

        A(x) = 1/( 1 / A(x^3)^2 - 4*x/(1-x) )^(1/2),

    which leads to the following recurrence for the coefficients a(n):

        a(0) = 1
        For n > 0:
            let b(n) = v_3(n) + 1  (exponent of 3 in n, plus one)
            c(n)   = 4 * Σ_{m=1..n} b(m) * c(n-m)
            a(n)   = ( c(n) - Σ_{k=1..n-1} a(k) * a(n-k) ) / 2

    The implementation below memoises the sequence in static arrays and
    grows them on demand so that calls to a(n) in any order are served
    in O(1) after the necessary initialisation.
*/

#include <stdlib.h>
#include "../utils/v3_plus_one.h"

static seq_t *a_cache = NULL;   /* a(n) values              */
static seq_t *c_cache = NULL;   /* auxiliary c(n)          */
static seq_t *b_cache = NULL;   /* auxiliary b(n)          */
static size_t cache_size = 0;   /* number of computed entries (0..cache_size‑1) */

/*  Ensure that the caches contain entries up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;      /* already computed */

    size_t new_size = (size_t)n + 1;

    a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    c_cache = (seq_t *)realloc(c_cache, new_size * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));

    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            c_cache[0] = (seq_t)1;
            b_cache[0] = (seq_t)0;    /* unused for i==0 */
        } else {
            /* compute b[i] */
            b_cache[i] = v3_plus_one((idx_t)i);

            /* compute c[i] = 4 * Σ_{m=1..i} b[m] * c[i-m] */
            seq_t sum_c = 0;
            for (size_t m = 1; m <= i; ++m) {
                sum_c += (seq_t)4 * b_cache[m] * c_cache[i - m];
            }
            c_cache[i] = sum_c;

            /* compute a[i] = (c[i] - Σ_{k=1..i-1} a[k]*a[i-k]) / 2 */
            seq_t sum_a = 0;
            for (size_t k = 1; k < i; ++k) {
                sum_a += a_cache[k] * a_cache[i - k];
            }
            a_cache[i] = (c_cache[i] - sum_a) / (seq_t)2;
        }
    }
    cache_size = new_size;
}

/*  Return the nth term of A382368.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
