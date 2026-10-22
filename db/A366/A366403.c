/*
 * A366403  G.f. A(x) satisfies A(x) = (1 + x / sqrt(A(x))) / (1 - x).
 * https://oeis.org/A366403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies A(x) = (1 + x / sqrt(A(x))) / (1 - x)
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 25, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build. */

/* Memoisation arrays for b = sqrt(A), a = A, and c = 1 / sqrt(A) */
static seq_t *b_arr = NULL;      /* b[n] = coefficient of sqrt(A)  */
static seq_t *a_arr = NULL;      /* a[n] = coefficient of A        */
static seq_t *c_arr = NULL;      /* c[n] = coefficient of 1/sqrt(A)*/
static idx_t  computed = 0;      /* number of elements currently computed (size of arrays) */

/* Ensure that a_arr, b_arr and c_arr contain entries 0..n inclusive. */
static void ensure_computed(idx_t n)
{
    if (n + 1 <= computed) return;          /* already computed */

    size_t new_size  = (size_t)(n + 1);
    size_t old_size  = (size_t)computed;

    /* Allocate or grow the arrays */
    b_arr = (seq_t *)realloc(b_arr, new_size * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    c_arr = (seq_t *)realloc(c_arr, new_size * sizeof(seq_t));

    /* Initialise new elements to 0 */
    for (size_t i = old_size; i < new_size; ++i) {
        b_arr[i] = 0;
        a_arr[i] = 0;
        c_arr[i] = 0;
    }

    /* Base case initialisation if this is the first call */
    if (old_size == 0) {
        b_arr[0] = 1;          /* b(0) = sqrt(A(0)) = 1   */
        a_arr[0] = 1;          /* a(0) = 1                 */
        c_arr[0] = 1;          /* c(0) = 1 / b(0) = 1      */
        computed = 1;
        old_size = 1;
    }

    /* Compute new entries from old_size up to n */
    for (idx_t idx = old_size; idx <= n; ++idx) {
        /* a[n] = a[n-1] + c[n-1]   (from (1-x)A = 1 + x / sqrt(A)) */
        seq_t a_val = a_arr[idx-1] + c_arr[idx-1];
        a_arr[idx] = a_val;

        /* Compute S = sum_{i=1}^{n-1} b[i] * b[n-i] */
        seq_t S = 0;
        for (idx_t i = 1; i <= idx-1; ++i) {
            S += b_arr[i] * b_arr[idx - i];
        }

        /* b[n] = (a[n] - S) / 2   (since a[n] = 2*b[n] + S) */
        seq_t b_val = (a_val - S) / 2;
        b_arr[idx] = b_val;

        /* c[n] = - sum_{k=1}^{n} b[k] * c[n-k]  (from B*C = 1) */
        seq_t c_sum = 0;
        for (idx_t k = 1; k <= idx; ++k) {
            c_sum += b_arr[k] * c_arr[idx - k];
        }
        c_arr[idx] = -c_sum;
    }

    computed = new_size;
}

/* Public function requested by the platform */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return a_arr[n];
}
