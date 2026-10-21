/*
 * A260301  Expansion of f(-x^3)^3 * psi(x)^3 / psi(x^3)^2 in powers of x
 *          where phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Euler transform of the period‑6 sequence b(k) = 3, -3,
 *            -2, -3, 3, -4, 3, -3, … That is, let S(k) = Σ_{d|k} d·b(d) then
 *            a(0)=1 and for n≥1 a(n) = (1/n)·Σ_{k=1}^{n} S(k)·a(n−k)
 * Range:     i8 n <= 10, i16 n <= 123, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* period‑6 sequence b(k) */
static inline int b_of(idx_t d)
{
    switch ((int)(d % 6))
    {
        case 1: return 3;
        case 2: return -3;
        case 3: return -2;
        case 4: return -3;
        case 5: return 3;
        default:            /* d % 6 == 0 */
            return -4;
    }
}

/* compute S[k] = Σ_{d|k} d * b(d) for k=1..L  */
static void compute_S(size_t L, seq_t *S)
{
    for (size_t i = 1; i <= L; ++i) S[i] = 0;
    for (size_t d = 1; d <= L; ++d)
    {
        int bd = b_of(d);
        seq_t add = (seq_t)d * (seq_t)bd;
        for (size_t m = d; m <= L; m += d) S[m] += add;
    }
}

/* global caches */
static seq_t *a_cache   = NULL;
static seq_t *S_cache   = NULL;
static size_t a_cap     = 0;
static size_t S_cap     = 0;
static size_t computed_up = 0;   /* next index not yet computed */

/* public routine */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* a(0)=1 */

    size_t N = (size_t)n;

    /* resize caches if necessary */
    if (N + 1 > a_cap || N + 1 > S_cap)
    {
        size_t new_cap = (N + 1) * 2;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        S_cache = (seq_t*)realloc(S_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
        S_cap   = new_cap;
    }

    /* recompute S up to needed limit */
    static size_t max_S = 0;
    if (N > max_S)
    {
        max_S = N;
        compute_S(max_S, S_cache);
    }

    /* compute a(n) values up to N */
    for (size_t cur = computed_up; cur <= N; ++cur)
    {
        if (cur == 0)
        {
            a_cache[0] = (seq_t)1;
        }
        else
        {
            seq_t sum = 0;
            for (size_t k = 1; k <= cur; ++k)
                sum += S_cache[k] * a_cache[cur - k];
            a_cache[cur] = sum / (seq_t)cur;
        }
    }
    computed_up = N + 1;

    return a_cache[N];
}
