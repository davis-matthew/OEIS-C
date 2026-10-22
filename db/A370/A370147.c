/*
 * A370147  Expansion of ( (1 + x)*(1 + 7*x)*(1 + 49*x) )^(1/3).
 * https://oeis.org/A370147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A370147.  The coefficients a(n) of the power series
   A(x) with A(x)^3 = (1 + x)(1 + 7x)(1 + 49x).
   Recurrence (derived from the cubic equation):

   Let B(k) = Σ_{i=0}^k a(i) a(k-i)   (convolution of a with itself).
   For n = 0:  a(0) = 1, B(0) = 1.
   For n ≥ 1:
       D(n)   = Σ_{i=1}^{n-1} a(i) a(n-i)          (no a(n) term)
       S(n)   = Σ_{k=1}^{n-1} a(k) B(n-k)          (uses only lower B)
       c_n    = 1   (n=0)
                57  (n=1)
                399 (n=2)
                343 (n=3)
                0   (n≥4)
       Then
           a(n) = (c_n - D(n) - S(n)) / 3
           B(n) = 2*a(n) + D(n)          (convolution value for future steps)
   This recurrence uses only previous terms, so it can be evaluated
   sequentially in O(n²) time (≤ 10⁸ operations for n≈10⁴).
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;      /* computed a values */
static seq_t *b_cache = NULL;      /* computed B values */
static idx_t   cacheSize = 0;      /* allocated array size */
static idx_t   nextIdx   = 0;      /* first unused index in caches */

/* Grow the caches to hold all indices up to 'needed' (inclusive). */
static void grow_cache(idx_t needed)
{
    idx_t required = needed + 1;
    if (required <= cacheSize) return;

    idx_t newSize = (cacheSize == 0) ? 1 : cacheSize;
    while (newSize < required) newSize <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_cache, newSize * sizeof(seq_t));
    seq_t *new_b = (seq_t *)realloc(b_cache, newSize * sizeof(seq_t));
    if (!new_a || !new_b) return;   /* allocation failed – leave unchanged */

    /* initialise new elements to zero */
    for (idx_t i = cacheSize; i < newSize; ++i) {
        new_a[i] = 0;
        new_b[i] = 0;
    }

    a_cache = new_a;
    b_cache = new_b;
    cacheSize = newSize;
}

/* Compute all terms up to 'n' (if not already computed).      */
static void compute_up_to(idx_t n)
{
    if (n < nextIdx) return;   /* already available */

    /* initialise caches on the very first call */
    if (a_cache == NULL) {
        grow_cache(n);
        a_cache[0] = 1;
        b_cache[0] = 1;
        nextIdx = 1;           /* only a[0] is known so far */
    }

    /* ensure enough space for required indices */
    grow_cache(n);

    for (idx_t cur = nextIdx; cur <= n; ++cur) {
        /* D(cur) = Σ_{i=1}^{cur-1} a[i] * a[cur-i] */
        seq_t D = 0;
        for (idx_t i = 1; i < cur; ++i) {
            D += a_cache[i] * a_cache[cur - i];
        }

        /* S(cur) = Σ_{k=1}^{cur-1} a[k] * B[cur-k] */
        seq_t S = 0;
        for (idx_t k = 1; k < cur; ++k) {
            S += a_cache[k] * b_cache[cur - k];
        }

        /* constant term c_cur */
        seq_t c = 0;
        switch (cur) {
        case 1: c = (seq_t)57; break;
        case 2: c = (seq_t)399; break;
        case 3: c = (seq_t)343; break;
        default: c = 0; break;
        }

        /* a[cur] = (c - D - S) / 3 */
        seq_t a_cur = (c - D - S) / (seq_t)3;

        /* B(cur) = 2*a[cur] + D  (convolution of a with itself at index cur) */
        seq_t b_cur = (seq_t)2 * a_cur + D;

        a_cache[cur] = a_cur;
        b_cache[cur] = b_cur;
    }

    nextIdx = n + 1;    /* all indices up to n are now available */
}

extern seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
