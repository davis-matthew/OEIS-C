/*
 * A289974  p-INVERT of the upper Wythoff sequence (A001950), where p(S) = 1 -
 *          S.
 * https://oeis.org/A289974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A289974: a(n) is the p‑INVERT of the upper Wythoff sequence
   (A001950) with p(S) = 1 – S.  For the upper Wythoff numbers
   c(n) = ⌊(n+1)·φ²⌋ (φ = (1+√5)/2).  
   The generating‑function equation
          A(x) = C(x) + x C(x) A(x)
   yields the recurrence
          a(0)   = c(0)
          a(n)   = c(n) + Σ_{k=0}^{n-1} c(k)·a(n-1-k)   (n ≥ 1)   */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *a_cache   = NULL;   /* a[0 .. max_n]          */
static seq_t *c_cache   = NULL;   /* c[0 .. max_n]          */
static size_t   cap      = 0;      /* current allocation size*/
static idx_t    max_n    = (idx_t)-1; /* last index for which a[max_n] is valid */

/* Transform only needed for building c(n).  Using long double gives enough
   precision for all n ≤ 10000. */
static const long double phi2 = (3.0L + sqrtl(5.0L)) / 2.0L;

/* Ensure the arrays are large enough for indices 0..n.  Allocate as
   needed; reallocate on demand.  Return 0 on allocation failure. */
static int ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap)
        return 1;

    size_t newcap = (size_t)(n + 1);
    seq_t *a_tmp = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    if (!a_tmp)
        return 0;
    a_cache = a_tmp;

    seq_t *c_tmp = (seq_t *)realloc(c_cache, newcap * sizeof(seq_t));
    if (!c_tmp)
        return 0;
    c_cache = c_tmp;

    cap = newcap;
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (max_n != (idx_t)-1 && n <= max_n)
        return a_cache[n];

    /* Ensure we have space for up to n. */
    if (!ensure_capacity(n))
        return 0;           /* allocation failure */

    /* Compute c(i) and a(i) for i from max_n+1 (or 0) up to n. */
    idx_t start = (max_n == (idx_t)-1) ? 0 : max_n + 1;
    for (idx_t i = start; i <= n; ++i)
    {
        /* Upper Wythoff term c(i) = floor((i+1)*φ²). */
        long double val = (long double)(i + 1) * phi2;
        idx_t ival = (idx_t)(val + 1e-12L);   /* guard against rounding */
        c_cache[i] = (seq_t)ival;
    }

    for (idx_t i = start; i <= n; ++i)
    {
        /* a(i) = c(i) + Σ_{k=0}^{i-1} c(k)·a(i-1-k)     */
        seq_t sum = c_cache[i];
        for (idx_t k = 0; k < i; ++k)
            sum += c_cache[k] * a_cache[i - 1 - k];
        a_cache[i] = sum;
    }

    max_n = n;            /* update highest computed index */
    return a_cache[n];
}
