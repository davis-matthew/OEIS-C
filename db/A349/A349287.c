/*
 * A349287  G.f. A(x) satisfies: A(x) = 1 / (1 - x * A(4*x)^2).
 * https://oeis.org/A349287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349287:
   a(0) = 1
   a(n) = Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1} 4^(i+j) * a(i) * a(j) * a(n-i-j-1)   (n≥1)
   (Equivalent: A(x) = 1 / (1 - x*A(4x)^2)) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow4.h"

static seq_t *a_vals = NULL;   /* a[n] */
static seq_t *c_vals = NULL;   /* c[n] = 4^n * a[n] */
static seq_t *z_vals = NULL;   /* z[k] = Σ_{t=0}^{k} c[t] * c[k-t] */
static size_t  cap   = 0;      /* current allocated size (in entries) */
static idx_t   last  = (idx_t)-1;   /* highest n that has been computed */

/* Ensure the arrays are large enough to contain indices up to n */
static void ensure_capacity(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_vals = (seq_t *)realloc(a_vals, (new_cap + 1) * sizeof(seq_t));
        c_vals = (seq_t *)realloc(c_vals, (new_cap + 1) * sizeof(seq_t));
        z_vals = (seq_t *)realloc(z_vals, (new_cap + 1) * sizeof(seq_t));
        /* initialise new elements to 0 */
        for (size_t i = cap; i <= new_cap; ++i) {
            a_vals[i] = 0;
            c_vals[i] = 0;
            z_vals[i] = 0;
        }
        cap = new_cap;
    }
}

static void initialise_sequence(void)
{
    if (last == (idx_t)-1) {           /* first call – initialise a[0] … z[0] */
        ensure_capacity(0);
        a_vals[0] = 1;
        c_vals[0] = 1;                 /* 4^0 * a[0] */
        z_vals[0] = 1;                 /* c[0]*c[0] */
        last = 0;
    }
}

seq_t a(idx_t n)
{
    initialise_sequence();
    if (n <= last)
        return a_vals[n];

    ensure_capacity(n);
    for (idx_t idx = last + 1; idx <= n; ++idx) {
        /* Compute a[idx] */
        seq_t sum = 0;
        for (idx_t k = 0; k < idx; ++k) {
            sum += z_vals[k] * a_vals[idx - k - 1];
        }
        a_vals[idx] = sum;

        /* Compute c[idx] = 4^idx * a[idx] */
        seq_t pow4_val = pow4(idx);
        c_vals[idx] = pow4_val * a_vals[idx];

        /* Compute z[idx] = Σ_{t=0}^{idx} c[t] * c[idx-t] */
        seq_t zsum = 0;
        for (idx_t t = 0; t <= idx; ++t) {
            zsum += c_vals[t] * c_vals[idx - t];
        }
        z_vals[idx] = zsum;
    }
    last = n;
    return a_vals[n];
}
