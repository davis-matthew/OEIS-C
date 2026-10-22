/*
 * A355794  Row 1 of A355793.
 * https://oeis.org/A355794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A355794 (Row 1 of A355793):
 * Let t(0)=1 and t(k)=∏_{i=1..k}(3*i-1) for k≥1.
 * Then a(0)=1 and for n≥1
 *     a(n)=t(n+1)/2 – ∑_{j=1..n} a(n-j)·t(j) .
 * This implements the OGF relation
 *    A(x)=∑_{k≥0}t(k+1)/2·x^k ÷ ∑_{k≥0}t(k)·x^k .
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_vals   = NULL; /* cached values a(n)   */
static size_t   a_cap   = 0;    /* allocated size for a_vals   */
static seq_t *t_vals   = NULL; /* values t(k)   */
static size_t   t_cap   = 0;    /* allocated size for t_vals   */
static idx_t   max_computed = (idx_t)-1; /* highest n for which a(n) is known */

/* Ensure a_vals can store indices up to n (inclusive). */
static void ensure_a_size(idx_t n)
{
    if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_ptr = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE);
        a_vals = new_ptr;
        a_cap  = new_cap;
    }
}

/* Ensure t_vals can store indices up to n (inclusive). */
static void ensure_t_size(idx_t n)
{
    if (t_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_ptr = (seq_t*)realloc(t_vals, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE);

        /* If this is the first allocation, or we are expanding */
        if (!t_vals) { /* first allocation: t_vals is NULL */
            for (idx_t i = 0; i <= n; ++i) {
                if (i == 0)
                    new_ptr[0] = (seq_t)1;
                else
                    new_ptr[i] = new_ptr[i-1] * (seq_t)(3*i - 1);
            }
        } else { /* expanding: keep existing values, compute new ones */
            for (idx_t i = t_cap; i <= n; ++i)
                new_ptr[i] = new_ptr[i-1] * (seq_t)(3*i - 1);
        }

        t_vals = new_ptr;
        t_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* First call: initialise storage and compute up to n. */
    if (max_computed == (idx_t)-1) {
        ensure_a_size(n);
        ensure_t_size(n + 1);   /* need t(0)…t(n+1) */
        a_vals[0] = (seq_t)1;
        max_computed = 0;
    }

    /* If the desired value is already cached, return it. */
    if (n <= max_computed)
        return a_vals[n];

    /* Extend the cache to n. */
    ensure_a_size(n);
    ensure_t_size(n + 1);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t tn1_div2 = t_vals[i + 1] / (seq_t)2;
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j) {
            sum += t_vals[j] * a_vals[i - j];
        }
        a_vals[i] = tn1_div2 - sum;
    }

    max_computed = n;
    return a_vals[n];
}
