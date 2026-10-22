/*
 * A363816  G.f. satisfies A(x) = (1 + x/A(x))/(1 - x)^2.
 * https://oeis.org/A363816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A(x) = -2*x / (1-sqrt(1+4*x*(1-x)^2))
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 29, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the functional equation) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;
    static seq_t *s_vals  = NULL;      /* s_k = sum_{i=0}^k a[i]*a[k-i] */
    static idx_t capacity = 0;
    static idx_t max_computed = 0;     /* highest index whose value is known */

    /* Ensure we have space for the requested index */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap <= n + 1)
            new_cap *= 2;
        seq_t *tmp_a = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        seq_t *tmp_s = (seq_t *)realloc(s_vals, new_cap * sizeof(seq_t));
        if (!tmp_a || !tmp_s) return 0; /* allocation failure (should not happen) */
        a_vals = tmp_a;
        s_vals = tmp_s;
        /* initialize the new entries implicitly to zero by the compiler’s behaviour,
           but we set them explicitly to be safe */
        for (idx_t i = capacity; i < new_cap; ++i) {
            a_vals[i] = 0;
            s_vals[i] = 0;
        }
        capacity = new_cap;
    }

    /* Initialize the base cases if not yet done */
    if (max_computed == 0) {
        a_vals[0] = 1;
        s_vals[0] = 1;                /* a[0]*a[0] = 1 */
        max_computed = 0;
    }

    if (n == 0)
        return a_vals[0];

    /* Compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1) {
            /* Base value given by the sequence */
            a_vals[1] = 3;
            /* s[1] = a[0]*a[1] + a[1]*a[0] */
            s_vals[1] = a_vals[0] * a_vals[1] + a_vals[1] * a_vals[0];
        } else {
            /* Compute S = sum_{i=1}^{k-1} a[i] * a[k-i] */
            seq_t S = 0;
            for (idx_t i = 1; i < k; ++i) {
                S += a_vals[i] * a_vals[k - i];
            }
            /* a[k] = 2*s[k-1] - s[k-2] - S */
            seq_t a_k = 2 * s_vals[k - 1] - s_vals[k - 2] - S;
            a_vals[k] = a_k;

            /* Compute s[k] = sum_{i=0}^{k} a[i]*a[k-i] */
            seq_t sk = 0;
            for (idx_t i = 0; i <= k; ++i) {
                sk += a_vals[i] * a_vals[k - i];
            }
            s_vals[k] = sk;
        }
    }
    max_computed = n;

    return a_vals[n];
}
