/*
 * A377243  Expansion of sqrt(1 - 4*x*sqrt(1 - 4*x)).
 * https://oeis.org/A377243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4^n * Sum_{k=0..n} (-1)^k * binomial(1/2,k) *
 *            binomial(n-3*k/2-1, n-k)
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 17, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL; /* a[i] = a(i) */
static seq_t *b_cache = NULL; /* b[i] = Σ a(j)*a(i-j)  */
static size_t cache_cap = 0;  /* allocated size of the caches */
static idx_t  last_computed = (idx_t)(-1);

/* Grow caches to accommodate indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *new_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        a_cache = new_a;
        b_cache = new_b;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A377243. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    ensure_capacity(n);

    /* Initialise caches if first call */
    if (last_computed == (idx_t)(-1)) {
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Compute values up to n */
    while (last_computed < n) {
        idx_t idx = last_computed + 1;           /* next index to compute */
        if (idx == 1) {                          /* a(1) */
            b_cache[1] = (seq_t)-4;              /* b1 = 2*a1 -> a1 = -2 */
            a_cache[1] = b_cache[1] / (seq_t)2;
        } else {
            /* Equation index: n_equ = idx + 1  (rhs defined for n_equ) */
            idx_t n_equ = idx + 1;
            seq_t rhs;
            if (n_equ == 2)   rhs = (seq_t)16;          /* unused here, handled above */
            else if (n_equ == 3) rhs = (seq_t)-64;
            else             rhs = (seq_t)0;

            /* Sum S = Σ_{i=2}^{n_equ-2} b[i]*b[n_equ-i]  (empty if n_equ<=3) */
            seq_t S = 0;
            if (n_equ >= 4) {
                for (idx_t i = 2; i <= n_equ - 2; ++i) {
                    S += b_cache[i] * b_cache[n_equ - i];
                }
            }

            /* Compute b[idx] (which is b[n_equ-1]) */
            seq_t b_val;
            if (n_equ == 3) {
                /* b2 = rhs / (2*b1)   (b1 = -4) */
                b_val = rhs / ((seq_t)2 * b_cache[1]);
            } else {
                /* For n_equ >= 4 : b[idx] = (rhs - S) / (2*b1)  (rhs = 0) */
                b_val = (rhs - S) / ((seq_t)2 * b_cache[1]);
            }
            b_cache[idx] = b_val;

            /* Compute a[idx] from b[idx] */
            seq_t sumA = 0;
            for (idx_t i = 1; i < idx; ++i) {
                sumA += a_cache[i] * a_cache[idx - i];
            }
            a_cache[idx] = (b_cache[idx] - sumA) / (seq_t)2;
        }
        last_computed = idx;
    }

    return a_cache[n];
}
