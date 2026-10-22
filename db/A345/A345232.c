/*
 * A345232  G.f. A(x) satisfies: A(x) = x + x^2 / exp(A(x) + A(x^2)/2 +
 *          A(x^3)/3 + A(x^4)/4 + ...).
 * https://oeis.org/A345232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 39, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A345232
   a(1) = 1,      a(2) = 1
   For n ≥ 1
     a(n+2) = -(1/n) * Σ_{k=1..n} ( Σ_{d|k} d * a(d) ) * a(n-k+2)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helper: ensure that the arrays a[0..cap-1] and b[0..cap-1] have
   capacity at least need.  The arrays are reallocated as needed.
   After reallocation, the divisor‑sum array b is rebuilt from scratch. */
static void ensure_cap(size_t need, seq_t **a, seq_t **b, size_t *cap, size_t max_idx)
{
    if (*cap >= need) return;

    size_t new_cap = (*cap) ? (*cap) << 1 : 4;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_a || !new_b) abort();     /* memory failure */

    /* copy existing a[0..*cap-1] */
    for (size_t i = 0; i < *cap; ++i) new_a[i] = (*a)[i];
    /* initialize a beyond old capacity to zero */
    for (size_t i = *cap; i < new_cap; ++i) new_a[i] = 0;
    /* zero b array first */
    for (size_t i = 0; i < new_cap; ++i) new_b[i] = 0;

    /* rebuild divisor‑sum array b from known a[*] values */
    for (size_t d = 1; d <= max_idx; ++d)
    {
        seq_t contrib = (seq_t)d * (*a)[d];
        for (size_t m = d; m < new_cap; m += d) new_b[m] += contrib;
    }

    /* free old arrays and commit new ones */
    free(*a); free(*b);
    *a = new_a; *b = new_b; *cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* out of range: no defined value */

    /* static storage for sequence and auxiliary divisor sums */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static size_t cap   = 0;        /* allocated capacity */
    static size_t max_idx = 0;      /* highest computed index in a_arr (≥2) */
    static bool init = false;

    if (!init)
    {
        /* allocate minimal space enough for a[0..2] and b[0..2] */
        ensure_cap(3, &a_arr, &b_arr, &cap, 0);
        /* base values */
        a_arr[1] = (seq_t)1;
        a_arr[2] = (seq_t)1;
        max_idx = 2;

        /* rebuild b_arr for known a[1], a[2] */
        for (size_t d = 1; d <= max_idx; ++d)
        {
            seq_t contrib = (seq_t)d * a_arr[d];
            for (size_t m = d; m < cap; m += d) b_arr[m] += contrib;
        }
        init = true;
    }

    /* compute up to requested index if necessary */
    if ((size_t)n > max_idx)
    {
        size_t need = (size_t)n + 1;                     /* A(n) is indexed at n, so array needs size n+1 */
        ensure_cap(need, &a_arr, &b_arr, &cap, max_idx);

        for (uint64_t d = max_idx + 1; d <= (uint64_t)n; ++d)
        {
            if (d <= 2) /* already set; keep for completeness */
            {
                /* nothing to do */
            }
            else
            {
                uint64_t N = d - 2;                    /* n in the recurrence */
                seq_t sum = 0;
                /* Σ_{k=1..N} b[k] * a[d - k]  */
                for (uint64_t k = 1; k <= N; ++k)
                    sum += b_arr[k] * a_arr[d - k];

                /* a(d) = -(sum) / N  */
                a_arr[d] = -sum / (seq_t)N;
            }

            /* update divisor‑sum array for multiples of d */
            seq_t contrib = (seq_t)d * a_arr[d];
            for (size_t m = d; m < cap; m += d)
                b_arr[m] += contrib;
        }
        max_idx = (size_t)n;
    }

    return a_arr[n];
}
