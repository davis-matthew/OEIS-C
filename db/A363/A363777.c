/*
 * A363777  Expansion of 1/(Sum_{k>=0} x^(k^3))^3.
 * https://oeis.org/A363777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 38, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence for OEIS A363777:
   a(n) = coefficient of x^n in 1/(∑_{k≥0} x^{k^3})^3.
   Let b(n) = 1 if n is a perfect cube, 0 otherwise.
   Then c(n) = (b*b*b)(n) = #{i,j,k≥0 | i^3 + j^3 + k^3 = n}.
   The sequence satisfies a(0)=1 and for n≥1
        a(n) = –∑_{k=0}^{n-1} a(k) · c(n–k).
   The code stores c(n) and a(n) in static arrays and memoises results
   up to the largest n requested. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *c_arr = NULL;          /* c(n) = number of representations as sum of three cubes */
static seq_t *a_arr = NULL;          /* a(n) values */
static size_t arr_cap   = 0;         /* current capacity of the arrays */
static size_t max_n_computed = 0;    /* largest n for which a_arr[0..n] are valid */

static void ensure_arrays(idx_t n)
{
    /* Resize arrays if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > arr_cap) {
        size_t new_cap = needed;
        c_arr = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        for (size_t i = arr_cap; i < new_cap; ++i) {
            c_arr[i] = 0;
            a_arr[i] = 0;
        }
        arr_cap = new_cap;
    }

    /* Initialise a(0) if this is the first time */
    if (max_n_computed == 0 && arr_cap > 0) {
        a_arr[0] = (seq_t)1;
    }

    /* If n is not larger than everything already computed, nothing to do */
    if (n <= (idx_t)max_n_computed) {
        return;
    }

    /* Recompute c_arr for all indices up to n */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        c_arr[i] = 0;
    }

    /* Pre‑compute the list of cubes ≤ n */
    idx_t max_i = 0;
    for (idx_t i = 0; ; ++i) {
        idx_t cube = i * i * i;
        if (cube > n) break;
        max_i = i;
    }

    /* Count representations as sums of three cubes */
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t ci = i * i * i;
        for (idx_t j = 0; j <= max_i; ++j) {
            idx_t cj = j * j * j;
            idx_t sumij = ci + cj;
            if (sumij > n) continue;
            for (idx_t k = 0; k <= max_i; ++k) {
                idx_t ck = k * k * k;
                idx_t s = sumij + ck;
                if (s > n) continue;
                c_arr[(size_t)s] += (seq_t)1;
            }
        }
    }

    /* Compute a(n) values incrementally from the last computed index */
    for (size_t cur = max_n_computed + 1; cur <= (size_t)n; ++cur) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < cur; ++k) {
            sum += a_arr[k] * c_arr[cur - k];
        }
        a_arr[cur] = -(sum);
    }

    max_n_computed = (size_t)n;
}

seq_t a(idx_t n)
{
    ensure_arrays(n);
    return a_arr[(size_t)n];
}
