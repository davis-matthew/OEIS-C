/*
 * A255928  Expansion of exp( Sum_{n >= 1} A094088(n)*x^n/n ).
 * https://oeis.org/A255928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1: a(n) = (1/n) * Σ_{k=0}^{n-1} A094088(n-k)
 *            * a(k) where A094088(0)=1 and A094088(n) = Σ_{k=0}^{n-1}
 *            binom(2*n,2*k) * A094088(k). Values are computed and memoized in
 *            static arrays
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *u_arr = NULL;          /* A094088 values */
static seq_t *a_arr = NULL;          /* A255928 values */
static idx_t last_computed = 0;      /* highest n for which u_arr and a_arr are defined */

/* Ensure that u_arr[k] and a_arr[k] are defined for all k <= n */
static void ensure_sequences(idx_t n)
{
    if (n == 0) {                     /* a(0)=1, u(0)=1 already defined when first needed */
        if (u_arr == NULL) {
            u_arr = seq_alloc(1);
            a_arr = seq_alloc(1);
            u_arr[0] = 1;
            a_arr[0] = 1;
        }
        return;
    }

    if (u_arr != NULL) {
        if (n <= last_computed) return;            /* already computed up to n */
    }

    /* Allocate or enlarge arrays */
    size_t new_cap = (size_t)(n + 1);
    if (u_arr == NULL) {
        u_arr = seq_alloc(new_cap);
        a_arr = seq_alloc(new_cap);
        u_arr[0] = 1;
        a_arr[0] = 1;
    } else {
        u_arr = seq_realloc(u_arr, (size_t)(last_computed + 1), new_cap);
        a_arr = seq_realloc(a_arr, (size_t)(last_computed + 1), new_cap);
    }

    /* Compute new terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Compute u_arr[i] = Σ_{k=0}^{i-1} binom(2*i,2*k) * u_arr[k] */
        seq_t sum_u = 0;
        seq_t binom = 1;                      /* binom(2*i,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum_u += binom * u_arr[k];
            if (k + 1 < i) {                 /* update to binom(2*i,2*(k+1)) */
                seq_t num1 = (seq_t)(2 * i - 2 * k);
                seq_t num2 = (seq_t)(2 * i - 2 * k - 1);
                seq_t den1 = (seq_t)(2 * k + 2);
                seq_t den2 = (seq_t)(2 * k + 1);
                binom = binom * num1 * num2 / (den1 * den2);
            }
        }
        u_arr[i] = sum_u;

        /* Compute a_arr[i] = (Σ_{k=0}^{i-1} u_arr[i-k] * a_arr[k]) / i */
        seq_t sum_a = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum_a += u_arr[i - k] * a_arr[k];
        }
        a_arr[i] = sum_a / (seq_t)i;
    }

    last_computed = n;
}

seq_t a(idx_t n)
{
    ensure_sequences(n);
    return a_arr[n];
}
