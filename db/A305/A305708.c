/*
 * A305708  Expansion of e.g.f. exp(cos(x)/exp(x) - 1).
 * https://oeis.org/A305708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n/n! in the expansion of
 *            exp(cos(x)/exp(x) - 1). The recurrence used is a(0) = 1 a(n) =
 *            Σ_{k=1..n} C(n−1, k−1) · a(n−k) · b_k where b_k = (−1)^k ·
 *            Re[(1+i)^k], i.e. the real part of (1+i)^k multiplied by (−1)^k.
 *            The binomial coefficients C(n−1, k−1) are computed multi...
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Static caches for a(n) and for the real parts of (1+i)^k          */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;      /* a(0)..a(current) */
static size_t a_cap   = 0;          /* capacity of a_cache */

static seq_t *r_cache = NULL;      /* Re[(1+i)^k] for k=0..current */
static size_t r_cap   = 0;          /* capacity of r_cache */

/* Current real and imaginary parts for the next (1+i)^k calculation */
static seq_t r_real = 1;            /* real part of (1+i)^k at index r_cap-1 */
static seq_t r_imag = 0;            /* imaginary part of (1+i)^k at index r_cap-1 */

/* ------------------------------------------------------------------ */
/* Ensure that r_cache holds values up to index idx (inclusive)       */
/* ------------------------------------------------------------------ */
static void ensure_r_cache(idx_t idx)
{
    if (idx + 1 <= r_cap)
        return;

    size_t new_cap = (size_t)(idx + 1);
    r_cache = (seq_t*)realloc(r_cache, new_cap * sizeof(seq_t));
    if (!r_cache)
        abort();

    for (idx_t i = r_cap; i <= idx; ++i)
    {
        /* (1+i)^{i+1} = (1+i)^{i} * (1+i) */
        seq_t new_real = r_real - r_imag;
        seq_t new_imag = r_real + r_imag;
        r_cache[i] = new_real;
        r_real    = new_real;
        r_imag    = new_imag;
    }
    r_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that a_cache holds values up to index idx (inclusive)       */
/* ------------------------------------------------------------------ */
static void ensure_a_cache(idx_t idx)
{
    if (idx + 1 <= a_cap)
        return;

    /* Grow a_cache */
    size_t new_cap = (size_t)(idx + 1);
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!a_cache)
        abort();

    if (a_cap == 0)
    {
        a_cap = 1;
        a_cache[0] = 1;                /* a(0) = 1 */
    }

    for (idx_t m = a_cap; m <= idx; ++m)
    {
        seq_t sum   = 0;
        seq_t binom = 1;               /* C(m-1,0) */

        for (idx_t k = 1; k <= m; ++k)
        {
            /* b_k = (−1)^k * Re[(1+i)^k] */
            seq_t sign = (k & 1) ? (seq_t)(-1) : (seq_t)1;
            seq_t term = a_cache[m - k];
            term       = term * sign;
            term       = term * r_cache[k];
            term       = term * binom;
            sum        += term;

            if (k < m)                  /* prepare C(m-1,k) for next k */
            {
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k);
            }
        }
        a_cache[m] = sum;
    }

    a_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* First‑time initialisation of the caches */
    if (a_cache == NULL)
    {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;
        a_cap = 1;
    }

    if (r_cache == NULL)
    {
        r_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!r_cache) abort();
        r_cache[0] = 1;          /* Re[(1+i)^0] = 1 */
        r_cap = 1;
        r_real = 1;
        r_imag = 0;
    }

    /* Make sure the required values are available */
    ensure_r_cache(n);
    ensure_a_cache(n);

    return a_cache[n];
}
