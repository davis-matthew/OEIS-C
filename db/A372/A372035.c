/*
 * A372035  G.f. A(x) satisfies A(x) = ( 1 + 4*x*A(x)/(1 - x) )^(1/2).
 * https://oeis.org/A372035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) given by A(x) = sqrt(1 + 4*x*A(x)/(1-x)), a(0)=1
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 33, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for standard library functions */
#include <stdlib.h>

/* Global cached sequences and state */
static seq_t *b_cache   = NULL;  /* coefficients of sqrt(1-2x+5x^2) */
static seq_t *a_cache   = NULL;  /* required sequence */
static size_t capacity  = 0;     /* allocated length of the caches */
static size_t len       = 0;     /* number of already computed terms */

/* Ensure the caches contain terms up to index n */
static void compute_to(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (capacity < need) {
        size_t new_cap = capacity ? capacity << 1 : 1;
        if (new_cap < 3) new_cap = 3;          /* allow indices 0..2 safely */
        while (new_cap < need) new_cap <<= 1;
        b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    if (len == 0) {
        /* initialise first few b values */
        if (capacity >= 1) b_cache[0] = (seq_t)1;          /* b0 = 1  */
        if (capacity >= 2) b_cache[1] = (seq_t)-1;          /* b1 = -1 */
        if (capacity >= 3) b_cache[2] = (seq_t)2;          /* b2 =  2 */
        if (capacity >= 1) a_cache[0] = (seq_t)1;          /* a0 = 1  */
        len = 1;
    }

    /* Compute missing b terms */
    for (size_t k = len; k <= need - 1; ++k) {
        if (k < 3) continue;                           /* already set */
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += b_cache[i] * b_cache[k - i];
        }
        /* sum is even; divide exactly by 2 */
        b_cache[k] = -(sum / 2);
    }

    /* Compute missing a terms */
    for (size_t m = len; m <= need - 1; ++m) {
        seq_t rhs = (m == 1 ? (seq_t)-1 : (seq_t)0);
        seq_t sum_cross = 0;
        for (size_t k = 1; k <= m; ++k) {
            seq_t d = (k == 1 ? (seq_t)(b_cache[1] - ((seq_t)2))
                              : b_cache[k]);          /* d1 = b1-2, dk=bk for k>=2 */
            sum_cross += d * a_cache[m - k];
        }
        a_cache[m] = rhs - sum_cross;
    }

    len = need;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return a_cache[(size_t)n];
}
