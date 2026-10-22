/*
 * A373543  Expansion of 1/(1 - x/(1 - 9*x^2)^(2/3)).
 * https://oeis.org/A373543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 9^k * binomial(2*n/3 - k/3 - 1 , k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)= Σ_{m=0..⌊(n‑1)/2⌋} c[m] * a(n‑1‑2m),            */
/* where c[0]=1 and c[m]=c[m‑1]*3*(3*m‑1)/m for m≥1, the coefficients of (1‑9*x²)^(-2/3). */
#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build system. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static storage for the sequence a(n) and coefficients c[m]. */
    static seq_t *a_vals = NULL;
    static size_t a_cap = 0;          /* allocated capacity for a_vals */
    static size_t a_len = 0;          /* number of a-values already computed */

    static seq_t *c_vals = NULL;
    static size_t c_cap = 0;          /* allocated capacity for c_vals */
    static size_t c_len = 0;          /* number of c-values already computed (=c_len==c_cap most of the time) */

    /* --------------------------------------------------------------------
       Ensure capacity and initialise the base cases.
       -------------------------------------------------------------------- */
    /* Allocate/resize a_vals so that index n is valid. */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        a_vals = tmp;
        a_cap  = new_cap;
    }

    /* Ensure we have at least one a-value (a(0)=1). */
    if (a_len == 0) {
        a_vals[0] = 1;
        a_len = 1;
    }

    /* Allocate/resize c_vals to hold all coefficients up to
       m_max = ⌊(n‑1)/2⌋. */
    size_t m_max_needed = (size_t)((n > 0) ? ((n - 1) / 2) : 0);
    size_t c_needed = m_max_needed + 1;           /* include c[0] */
    if (c_needed > c_cap) {
        size_t new_cap = c_needed;
        seq_t *tmpc = (seq_t *)realloc(c_vals, new_cap * sizeof(seq_t));
        if (!tmpc) return 0;
        c_vals = tmpc;
        c_cap = new_cap;
    }

    /* Initialise coefficients if this is the first time. */
    if (c_len == 0) {
        c_vals[0] = 1;
        c_len = 1;
    }

    /* Compute any missing coefficients. */
    for (size_t m = c_len; m < c_needed; ++m) {
        /* c[m] = c[m‑1] * 3*(3*m‑1) / m  (exact division) */
        seq_t prev = c_vals[m - 1];
        seq_t numerator = prev * (seq_t)(3 * (3 * (idx_t)m - 1));
        c_vals[m] = numerator / (seq_t)m;
    }
    c_len = c_needed;

    /* --------------------------------------------------------------------
       Compute a-values up to n using the recurrence.
       -------------------------------------------------------------------- */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum = 0;
        size_t max_m = (size_t)((i - 1) / 2);
        for (size_t m = 0; m <= max_m; ++m) {
            idx_t j = i - 1 - 2 * (idx_t)m;
            sum += c_vals[m] * a_vals[j];
        }
        a_vals[i] = sum;
    }
    a_len = n + 1;

    return a_vals[n];
}
