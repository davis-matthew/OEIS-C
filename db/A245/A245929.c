/*
 * A245929  G.f.: Sum_{n>=0} x^n * Sum_{k=0..n} (-1)^k * C(n,k)^2 *
 *          Sum_{j=0..k} C(k,j)^2 * (-2*x)^j.
 * https://oeis.org/A245929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..2*n} C(2*k, k)*C(2*n+k, 2*n-k)^2*(-2)^(2*n-k)
 *            Recurrence: n^2*(4*n-5)*a(n) =
 *            -2*(4*n-3)*(68*n^2-102*n+21)*a(n-1) - 4*(2*n-3)^2*(4*n-1)*a(n-2)
 *            Initial values: a(0)=1, a(1)=-26. This function memoises the
 *            sequence in an internal cache and computes further terms by the
 *            recurrence
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* dynamic cache for computed terms   */
static size_t cache_cap = 0;     /* allocated capacity                 */
static size_t cache_size = 0;    /* number of terms currently stored   */

/* Compute a(n) – works for any n ≥ 0, memoised across calls. */
seq_t a(idx_t n)
{
    /* Quick return if already computed. */
    if (cache_size > (size_t)n)
        return cache[(size_t)n];

    /* Ensure the cache is large enough for n+1 terms. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache        = tmp;
        cache_cap    = new_cap;
    }

    /* Initialise the first two known terms if the cache is new. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-26;
        cache_size = 2;
    }

    /* Extend the cache up to index n (inclusive). */
    for (size_t idx = cache_size; idx <= (size_t)n; ++idx) {
        if (idx <= 1) continue;      /* base values already set   */

        idx_t i = (idx_t)idx;        /* use idx_t for arithmetic   */
        /* ---------- compute using recurrence --------- */
        if (sizeof(seq_t) <= 8) {    /* seq_t is 64‑bit or smaller */
            /* intermediate computations in 128‑bit */
            __int128 ii = ( __int128 )i;
            __int128 coeff1 = (4*ii - 3);
            __int128 poly = ( __int128 )68 * ii * ii - ( __int128 )102 * ii + 21;
            __int128 term1 = -2 * coeff1 * poly * (__int128)cache[idx-1];

            __int128 twom3 = (2*ii - 3);
            __int128 term2 = -4 * twom3 * twom3 * (4*ii - 1) * (__int128)cache[idx-2];

            __int128 numer = term1 + term2;
            __int128 denom = ii * ii * (4*ii - 5);

            cache[idx] = (seq_t)( numer / denom );
        } else {                     /* seq_t is 128‑bit or arbitrary‑precision */
            seq_t ii = (seq_t)i;
            seq_t coeff1 = (seq_t)(4*i - 3);
            seq_t poly = (seq_t)68 * ii * ii - (seq_t)102 * ii + (seq_t)21;
            seq_t term1 = (seq_t)-2 * coeff1 * poly * cache[idx-1];

            seq_t twom3 = (seq_t)(2*i - 3);
            seq_t term2 = (seq_t)-4 * twom3 * twom3 * (seq_t)(4*i - 1) * cache[idx-2];

            seq_t numer = term1 + term2;
            seq_t denom = ii * ii * (seq_t)(4*i - 5);

            cache[idx] = numer / denom;     /* exact division */
        }
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
