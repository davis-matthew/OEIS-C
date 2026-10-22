/*
 * A392210  G.f.: x + sq(x/(1-x) + sq(x/(1-2*x) + sq(x/(1-3*x) + sq(x/(1-4*x)
 *          + ...)))) where sq(x) = x^2, an infinitely nested square.
 * https://oeis.org/A392210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392210:  G.f.  A(x) = x + A(x/(1 - x))^2.
   The function computes the terms a(n) (n ≥ 1) using a dynamic
   recurrence derived from the functional equation.  It stores
   the sequence values and the current binomial row in static
   arrays so that calls are order‑independent.  The method runs in
   O(n²) time and uses only integer arithmetic.  */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_free_array.h"

/* static storage for the sequence and intermediate data */
static seq_t *a_arr      = NULL;
static seq_t *s_arr      = NULL;
static seq_t *binom_row  = NULL;   /* current binomial row C(row, k)   */
static size_t capacity   = 0;      /* allocated array length          */
static idx_t  computed_up_to = 0;  /* largest index that has been filled */

/* ensure the static arrays are large enough for indices up to n      */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);   /* index 0 … n                  */
    if (capacity < needed) {
        a_arr      = seq_realloc(a_arr, capacity, needed);
        s_arr      = seq_realloc(s_arr, capacity, needed);
        binom_row  = seq_realloc(binom_row, capacity, needed);
        capacity   = needed;
    }
}

/* initialisation of indices 0…1 */
static void initialise_once()
{
    a_arr[0]      = (seq_t)0;
    s_arr[0]      = (seq_t)0;
    binom_row[0]  = (seq_t)1;

    a_arr[1] = (seq_t)1;
    s_arr[1] = a_arr[1] * binom_row[0];   /* = 1 */
    computed_up_to = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but never called */
    ensure_capacity(n);

    if (computed_up_to == 0) initialise_once();

    /* compute terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        /* a[i] = sum_{k=1}^{i-1} s[k] * s[i-k] */
        seq_t sum_a = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            sum_a += s_arr[(size_t)k] * s_arr[(size_t)(i - k)];
        }
        a_arr[(size_t)i] = sum_a;

        /* update binomial row to row i-1 (length i) */
        binom_row[(size_t)(i - 1)] = (seq_t)1;            /* new end = 1   */
        for (size_t j = (size_t)(i - 2); j > 0; --j) {   /* Pascal upward */
            binom_row[j] += binom_row[j - 1];
        }
        /* binom_row[0] remains 1 */

        /* s[i] = sum_{m=1}^{i} a[m] * C(i-1, m-1)   (current binomial row) */
        seq_t sum_s = (seq_t)0;
        for (idx_t m = 1; m <= i; ++m) {
            sum_s += a_arr[(size_t)m] * binom_row[(size_t)(m - 1)];
        }
        s_arr[(size_t)i] = sum_s;

        computed_up_to = i;
    }

    return a_arr[(size_t)n];
}
