/*
 * A348299  Indices of 0 in A348295: numbers m such that Sum_{k=1..m}
 *          (-1)^(floor(k*(sqrt(2)-1))) = Sum_{k=1..m} (-1)^A097508(k) = 0.
 * https://oeis.org/A348299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 32, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348299:
   a(1) = 0
   a(2n)   = a(2n-1) + 4
   a(2n-1) = ceil( a(n) * (3 + 2 * sqrt(2)) ),  n ≥ 2
   All arithmetic is performed with the integer type seq_t.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated size of cache */
static size_t cache_len = 0;     /* number of terms stored */

/* High‑precision approximation of sqrt(2) as a convergent
   of the continued fraction.  p/q = 1607521/1136689
   The error satisfies |sqrt(2) - p/q| < 1/(2q^2),
   which is sufficient for all terms that fit in a 64‑bit
   integer (v ≤ 2·10¹¹ ⇒ v < 2q²). */
static const uint32_t P = 1607521;   /* numerator of sqrt(2) approximation */
static const uint32_t Q = 1136689;   /* denominator of sqrt(2) approximation */

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Reallocate to the required size */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache = tmp;
    cache_cap = needed;
}

/* Compute a(n) for any 1‑based index n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n=0, but harmless for safety */
    /* Grow the cache if necessary */
    ensure_cache((size_t)n);
    /* Compute any missing terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* 1‑based index of the term we are computing */
        idx_t idx = (idx_t)(i + 1);

        seq_t val;
        if (idx == 1) {
            /* a(1) = 0 */
            val = 0;
        } else if ((idx & 1) == 0) {
            /* Even index: a(2n) = a(2n-1) + 4 */
            val = cache[i - 1] + 4;          /* cache[i-1] == a(idx-1) */
        } else {
            /* Odd index > 1: a(2n-1) = ceil( a(n) * (3 + 2√2) ) */
            idx_t half = (idx + 1) / 2;           /* n = (idx+1)/2 */
            seq_t a_half = cache[half - 1];       /* a(n) */

            /* Compute ceil( a_half * (3 + 2√2) )
               =  3*a_half + ceil( 2*a_half*sqrt(2) )
               =  3*a_half + floor( 2*a_half*sqrt(2) ) + 1   (since the product is never integer) */
            seq_t v = a_half << 1;                   /* 2 * a_half */

            /* floor_sqrt = floor( v * sqrt(2) )
               = floor( v * P / Q )  (P/Q ≈ sqrt(2)) */
            seq_t floor_sqrt = (v * P) / Q;          /* integer division truncates */

            seq_t sum = a_half * 3 + floor_sqrt;     /* 3*a_half + floor(...) */
            val = sum + 1;                           /* ceil */
        }
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
