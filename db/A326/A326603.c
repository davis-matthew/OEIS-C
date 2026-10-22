/*
 * A326603  E.g.f.: Sum_{n>=0} (x^(2*n-1) + 1)^n * x^n * exp(-x^(2*n+1)) / n!,
 *          an even function.
 * https://oeis.org/A326603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A326603
 *
 * Formula: 
 * a(n) = (2n)! * Σ_{m=0}^{2n} Σ_{k=0}^{m}
 *           (-1)^{j} / (k! (m-k)! j!)
 *   where j = (2n - m - k*(2m-1)) / (2m+1),
 *   and the expression contributes only if the numerator is
 *   a non‑negative integer divisible by (2m+1).
 *
 * The value is computed by dynamic factorial table and integer
 * arithmetic only (no floating‑point).
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fact_tab = NULL;      /* factorial table: fact_tab[i] = i! */
static size_t fact_len = 0;         /* current allocated length of fact_tab */

/* Ensure fact_tab contains factorials up to max_n (inclusive). */
static void ensure_factorial_upto(idx_t max_n)
{
    size_t needed = (size_t)max_n + 1;     /* include max_n itself */
    if (needed <= fact_len)
        return;

    size_t newlen = (fact_len == 0) ? needed : fact_len * 2;
    if (newlen < needed)
        newlen = needed;

    seq_t *newtab = (seq_t *)realloc(fact_tab, newlen * sizeof(seq_t));
    if (!newtab) {
        /* Allocation failure – terminate the program. */
        exit(EXIT_FAILURE);
    }

    /* Extend the table. */
    size_t i;
    if (fact_len == 0) {
        newtab[0] = (seq_t)1;
        fact_len = 1;
        i = 1;
    } else {
        i = fact_len;
    }

    for (; i < newlen; ++i) {
        newtab[i] = newtab[i - 1] * (seq_t)i;
    }

    fact_tab = newtab;
    fact_len = newlen;
}

seq_t a(idx_t n)
{
    /* We need factorials up to (2n)! for the final scaling factor. */
    ensure_factorial_upto((idx_t)(2 * n));

    seq_t fac2n = fact_tab[(size_t)(2 * n)];
    seq_t sum = (seq_t)0;

    /* Iterate over m and k as described in the formula. */
    for (idx_t m = 0; m <= 2 * n; ++m) {
        for (idx_t k = 0; k <= m; ++k) {
            /* Compute R = 2n - m - k*(2m-1). */
            long long R = (long long)(2 * n) - (long long)m
                          - (long long)k * ((long long)2 * m - 1);

            if (R < 0)
                continue;

            long long denom = (long long)2 * m + 1;      /* 2m+1 */
            if (denom <= 0)
                continue;     /* m=0 handled below */

            if (R % denom != 0)
                continue;

            idx_t j = (idx_t)(R / denom);

            /* Term = fac2n / (k! (m-k)! j!) * (-1)^j. */
            seq_t term = fac2n;
            term /= fact_tab[(size_t)k];
            term /= fact_tab[(size_t)(m - k)];
            term /= fact_tab[(size_t)j];

            if (j & 1)
                term = -term;

            sum += term;
        }
    }

    return sum;
}
