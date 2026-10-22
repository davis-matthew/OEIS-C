/*
 * A306831  Expansion of e.g.f. exp(Sum_{k>=1} x^(k^2)*(1 + x^(k^2))/k^2).
 * https://oeis.org/A306831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1, a(n) = Σ_{k=1…n} [ w(k) × a(n−k) × (n−1)! /
 *            (n−k)! ], where w(k) = 1 if k is a perfect square, w(k) = 2 if k
 *            = 2 × t² for some integer t, w(k) = 0 otherwise
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root (floor) for unsigned 64‑bit values */
static idx_t
floor_isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod == v)
            return mid;
        if (prod < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ------------------------------------------------------------------ */
/* Workspace for memoised values */
static seq_t *a_vals  = NULL;        /* a[0 … cap-1]       */
static unsigned char *w_vals = NULL; /* w[0 … cap-1]       */
static idx_t a_cap = 0;              /* current capacity    */
static idx_t a_last = 0;             /* highest n computed  */

/* Ensure arrays are large enough for index n */
static void
ensure_capacity(idx_t n)
{
    if (n < a_cap) return;           /* already large enough */

    size_t new_cap = (size_t)(n + 1);

    a_vals  = (seq_t *)realloc(a_vals,  new_cap * sizeof(seq_t));
    w_vals  = (unsigned char *)realloc(w_vals, new_cap * sizeof(unsigned char));

    /* initialise new entries */
    for (idx_t i = a_cap; i <= n; ++i) {
        a_vals[i] = (seq_t)0;
        if (i == 0) {
            a_vals[0] = (seq_t)1;    /* a(0) = 1 */
        }
        /* compute w(i) */
        unsigned char w = 0;
        if (i != 0) {
            idx_t s = floor_isqrt_u64(i);
            if (s * s == i) {
                w = 1;                /* perfect square */
            } else if ((i & 1u) == 0u) { /* possible twice square */
                idx_t u = i >> 1;
                idx_t t = floor_isqrt_u64(u);
                if (t * t == u)
                    w = 2;            /* twice a square */
            }
        }
        w_vals[i] = w;
    }

    a_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_capacity(n);

    if (n <= a_last)
        return a_vals[n];

    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum  = (seq_t)0;
        seq_t mult = (seq_t)1;            /* mult = (i-1)! / (i-m)! for current m */

        for (idx_t m = 1; m <= i; ++m) {
            unsigned char w = w_vals[m];
            if (w) {
                seq_t term = a_vals[i - m];
                term *= mult;            /* term = a[i-m] * prod_{t=i-m+1}^{i-1} t */
                term *= (seq_t)w;        /* multiply by weight w(m) */
                sum += term;
            }
            if (m < i)                 /* update mult for next m */
                mult *= (seq_t)(i - m);
        }

        a_vals[i] = sum;
    }

    a_last = n;
    return a_vals[n];
}
