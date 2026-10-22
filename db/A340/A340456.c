/*
 * A340456  G.f.: Sum_{n>=0} x^n/(1 - x^(5*n+1)) - x^3*Sum_{n>=0} x^(4*n)/(1 -
 *          x^(5*n+4)).
 * https://oeis.org/A340456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A340456: coefficients of the series
 *  Q(x) = Sum_{n>=0} x^n/(1 - x^(5*n+1))  -  x^3*Sum_{n>=0} x^(4*n)/(1 - x^(5*n+4))
 *
 *  The function a(n) computes the coefficient of x^n in this power series.
 *
 *  Implementation logic:
 *    For each integer k >= 0 the first sum contributes +1 at indices
 *      n + k*(5*n+1)   ( 0 <= n <= k )
 *    The second sum contributes –1 at indices
 *      3 + 4*n + k*(5*n+4)   ( 0 <= n <= k )
 *
 *    The routine builds a temporary array of coefficients up to the
 *    required index, zeroing it for each call.  All integer arithmetic
 *    is performed on the user supplied type seq_t, which may be a
 *    built‑in integer or an arbitrary‑precision integer class.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_arr = NULL;          /* temporary coefficient array */
static size_t a_cap = 0;             /* currently allocated capacity */

/* Allocate or grow the coefficient array to hold indices up to 'need'.
 * New entries are zero‑initialised.  Handles both C11 and C++17 builds. */
static void ensure_capacity(size_t need)
{
    size_t required = need + 1;
    if (required <= a_cap) return;

    seq_t *new_arr = NULL;
#ifdef __cplusplus
    new_arr = new seq_t[required];
#else
    new_arr = (seq_t*)malloc(required * sizeof(seq_t));
#endif
    if (!new_arr) abort();

    /* zero‑initialise new elements; old elements will be overwritten anyway */
    for (size_t i = a_cap; i < required; ++i)
        new_arr[i] = 0;

#ifdef __cplusplus
    delete[] a_arr;
#else
    free(a_arr);
#endif
    a_arr = new_arr;
    a_cap = required;
}

/* public entry point */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_capacity(idx);

    /* clear the relevant part of the array */
    for (size_t i = 0; i <= idx; ++i)
        a_arr[i] = 0;

    /* accumulate contributions from both sums */
    for (size_t m = 0; m <= idx; ++m)
    {
        /* first sum: +1 at indices m + k*(5*m+1) */
        size_t step1 = 5 * m + 1;
        for (size_t pos = m; pos <= idx; pos += step1)
            a_arr[pos] += 1;

        /* second sum: –1 at indices 3 + 4*m + k*(5*m+4) */
        size_t start2 = 3 + 4 * m;
        if (start2 <= idx)
        {
            size_t step2 = 5 * m + 4;
            for (size_t pos = start2; pos <= idx; pos += step2)
                a_arr[pos] -= 1;
        }
    }

    return a_arr[idx];
}
