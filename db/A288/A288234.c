/*
 * A288234  Coefficients in the expansion of 1/([r]-[2*r]*x+[3*r]*x^2-...); [
 *          ]=floor, r=-1+sqrt(7).
 * https://oeis.org/A288234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let c[k] = floor((k+1)*r) with r = sqrt(7)-1.
   Then a[0] = 1. For n ≥ 1:
     let N  = Σ_{i=0}^{n-1} a[i] * c[n-i]   over indices (n-i) odd
     let P  = Σ_{i=0}^{n-1} a[i] * c[n-i]   over indices (n-i) even
     a[n] = N - P.
   This follows from the power‑series inverse of
     1 / (∑_{k≥0} c[k] (-x)^k).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer floor square root of a 64‑bit unsigned integer */
static inline seq_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62; /* highest possible bit of sqrt(2^64-1) */
    while (bit > 0) {
        uint64_t nxt = r | bit;
        if ((unsigned __int128)nxt * nxt <= x) {
            r = nxt;
        }
        bit >>= 1;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    /* static caches for coefficients c[k] and sequence a[k] */
    static seq_t *c = NULL;
    static size_t c_cap = 0;

    static seq_t *a_arr = NULL;
    static size_t a_cap = 0;
    static size_t a_max = 0;   /* highest index computed so far */

    /* required length */
    size_t need = (size_t)n + 1;

    /* grow coefficient cache if necessary */
    if (c_cap < need) {
        size_t new_cap = need;
        seq_t *new_c = (seq_t *)realloc(c, new_cap * sizeof(seq_t));
        if (!new_c) return 0;           /* silently return zero on OOM */
        c = new_c;
        c_cap = new_cap;
    }

    /* compute missing coefficients c[k] = floor((k+1)*r) */
    for (size_t k = a_cap; k < need; ++k) {
        uint64_t idx = (uint64_t)(k + 1);
        uint64_t val = 7ULL * idx * idx;          /* 7*(k+1)^2 fits in 64‑bit */
        seq_t sq = floor_sqrt_uint64(val);        /* floor((k+1)*sqrt(7)) */
        c[k] = sq - idx;                          /* floor((k+1)*r) */
    }

    /* grow sequence cache if necessary */
    if (a_cap < need) {
        size_t new_cap = need;
        seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!new_a) return 0;
        a_arr = new_a;
        a_cap = new_cap;
    }

    /* compute missing terms a[k] */
    if (a_max == 0) {
        a_arr[0] = 1;   /* a(0) = 1 */
        a_max = 1;
    }
    for (size_t m = a_max; m < need; ++m) {
        seq_t N = 0, P = 0;
        for (size_t i = 0; i < m; ++i) {
            size_t idx = m - i;
            seq_t term = a_arr[i] * c[idx];
            if (idx & 1) {
                N += term;
            } else {
                P += term;
            }
        }
        a_arr[m] = N - P;
    }
    a_max = need;

    return a_arr[(size_t)n];
}
