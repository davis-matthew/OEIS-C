/*
 * A255676  E.g.f. satisfies: A'(x) = (1 + A(x))*(1 + A(x)^2).
 * https://oeis.org/A255676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A255676: a(n) satisfies the e.g.f. differential equation
 *   A'(x) = (1 + A(x)) * (1 + A(x)^2)
 * 
 * The coefficients obey the recurrence
 *   a(1) = 1
 *   for n ≥ 2:
 *       let m = n - 1
 *       a(n) = a(m) + c(m) + Σ_{i=1}^{m-2} C(m,i) * a(i) * c(m-i)
 *   where
 *       c(k) = Σ_{i=1}^{k-1} C(k,i) * a(i) * a(k-i)
 *   and C(p,q) is the binomial coefficient.
 * 
 * The code computes the terms on demand, memoising all previously
 * computed values.  It works for arbitrary `seq_t` and `idx_t`
 * types provided by the build system.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – undefined behaviour otherwise */
    if (n < 1) return (seq_t)0;

    static seq_t *a_arr = NULL;      /* a[0] unused, a[1] = 1 */
    static size_t cap_a = 0;
    static idx_t max_computed = 0;   /* largest n whose a[n] is known */

    static seq_t *c_arr = NULL;      /* auxiliary values */
    static size_t cap_c = 0;

    /* Ensure capacity up to n */
    if (a_arr == NULL || cap_a <= (size_t)n) {
        size_t new_cap = (cap_a == 0) ? (n + 1) : cap_a * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        cap_a = new_cap;
    }
    if (c_arr == NULL || cap_c <= (size_t)n) {
        size_t new_cap = (cap_c == 0) ? (n + 1) : cap_c * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        c_arr = (seq_t*)realloc(c_arr, new_cap * sizeof(seq_t));
        cap_c = new_cap;
    }

    /* Initialise base case on first use */
    if (max_computed == 0) {
        a_arr[1] = (seq_t)1;        /* a(1) = 1 */
        c_arr[0] = (seq_t)1;        /* c(0) = 1 (unused in recurrence) */
        c_arr[1] = (seq_t)0;        /* c(1) = 0 */
        max_computed = 1;
    }

    /* Compute all terms up to n */
    for (idx_t idx = max_computed + 1; idx <= n; ++idx) {
        idx_t m = idx - 1;          /* m >= 1 */

        /* ---- compute a[idx] ---- */
        seq_t sum_triple = 0;
        if (m >= 2) {
            seq_t binom = 1;     /* C(m,0) = 1 */
            for (idx_t i = 1; i <= m - 2; ++i) {
                /* binom = C(m,i) from previous binom */
                binom = (binom * (seq_t)(m - i + 1)) / (seq_t)i;
                sum_triple += binom * a_arr[i] * c_arr[m - i];
            }
        }
        a_arr[idx] = a_arr[m] + c_arr[m] + sum_triple;

        /* ---- compute c[idx] for future use ---- */
        seq_t sum_c = 0;
        if (idx >= 2) {
            seq_t binom = 1;     /* C(idx,0) = 1 */
            for (idx_t i = 1; i <= idx - 1; ++i) {
                binom = (binom * (seq_t)(idx - i + 1)) / (seq_t)i;
                sum_c += binom * a_arr[i] * a_arr[idx - i];
            }
        }
        c_arr[idx] = sum_c;
    }

    max_computed = n;
    return a_arr[n];
}
