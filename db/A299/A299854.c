/*
 * A299854  G.f. S(x) satisfies C(x)^(1/2) - S(x)^(1/2) = 1 such that
 *          C'(x)*S(x)^(1/2) = S'(x)*C(x)^(1/2) = 72*x.
 * https://oeis.org/A299854
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299854
   Sequence computed from the generating function
   S(x) = u(x)^2 where u(x) satisfies
   2 u(x)^3 + 3 u(x)^2 = 108 x^2 ,  u(0)=0 , u'(0)=6
   The coefficients of u are u_1=6, and for k≥2
        u_k = (-3 * Σ_{i=2}^{k-1} u_i u_{k+1-i}
               -2 * Σ_{i=1}^{k-1} u_i v_{k+1-i}) / 36
   where v_n = coefficient of x^n in u^2 (i.e. a(n) for n≥2).
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *u = NULL;        /* coefficients of u, 1‑based */
static seq_t *v = NULL;        /* coefficients of u^2, indices ≥2 */
static idx_t cap_u = 0;
static idx_t cap_v = 0;
static idx_t built_u = 0;      /* largest u index computed  (≥1) */
static idx_t built_n = 0;      /* largest s index computed  (≥2) */

/* expand u array to at least index needed */
static void ensure_capacity_u(idx_t needed)
{
    if (cap_u > needed) return;
    idx_t new_cap = needed + 1;          /* need indices 0..needed */
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (u) {
        memcpy(new_arr, u, cap_u * sizeof(seq_t));
        free(u);
    }
    u = new_arr;
    cap_u = new_cap;
}

/* expand v array to at least index needed */
static void ensure_capacity_v(idx_t needed)
{
    if (cap_v > needed) return;
    idx_t new_cap = needed + 1;
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (v) {
        memcpy(new_arr, v, cap_v * sizeof(seq_t));
        free(v);
    }
    v = new_arr;
    cap_v = new_cap;
}

/* Build coefficients up to n (s[n] = v[n]) */
static void build_up_to(idx_t n)
{
    if (n < 2) return;

    idx_t max_u_needed = n - 1;   /* largest u index required */

    /* initialisation on first use */
    if (built_u == 0) {
        ensure_capacity_u(1);
        ensure_capacity_v(2);
        u[1] = 6;                          /* u₁ = 6 */
        v[2] = 36;                          /* v₂ = u₁² = 36 */
        built_u = 1;
        built_n = 2;
    }

    /* enlarge arrays if needed */
    ensure_capacity_u(max_u_needed);
    ensure_capacity_v(n);

    /* compute missing u_k and v_{k+1} */
    for (idx_t k = built_u + 1; k <= max_u_needed; ++k) {
        /* sum1 = Σ_{i=2}^{k-1} u_i u_{k+1-i} */
        seq_t sum1 = 0;
        for (idx_t i = 2; i <= k - 1; ++i) {
            sum1 += u[i] * u[k + 1 - i];
        }

        /* sum3 = Σ_{i=1}^{k-1} u_i v_{k+1-i} */
        seq_t sum3 = 0;
        for (idx_t i = 1; i <= k - 1; ++i) {
            sum3 += u[i] * v[k + 1 - i];
        }

        /* u_k = (-3*sum1 - 2*sum3) / 36 */
        seq_t numer = -3 * sum1 - 2 * sum3;
        seq_t u_k  = numer / 36;
        u[k] = u_k;

        /* v_{k+1} = 12*u_k + sum1  (since v_{k+1}=2*u1*u_k + sum1, u1=6) */
        seq_t v_val = 12 * u_k + sum1;
        v[k + 1] = v_val;
    }

    built_u = max_u_needed;
    if (built_n < n) built_n = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined below the offset, return 0 */
    build_up_to(n);
    return v[n];                  /* a(n) is the coefficient of x^n in S(x) */
}
