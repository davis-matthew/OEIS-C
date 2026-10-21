/*
 * A254612  Expansion of f(-x) * f(-x^9) in powers of x where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A254612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1−x^k)(1−x^{9k})
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            (same as the product f(-x) * f(-x^9) where f is the Ramanujan theta function).  */
/* The coefficients of ∏(1−x^k) are 0 except at generalized pentagonal numbers g = m(3m±1)/2 (m≥1), for which
   the coefficient is (-1)^m; the constant term is 1.  This product is then a convolution of those
   coefficients with themselves shifted by multiples of 9.  */

#include <stdlib.h>
#include <stdint.h>

/* dynamic array holding coefficients of ∏_{k>0}(1−x^k) */
static seq_t *d = NULL;
static size_t d_size = 0;   /* number of initialized entries */
static size_t d_cap  = 0;   /* current allocation capacity */

/* Ensure coefficients d[0..n] are available. */
static void ensure_d(idx_t n)
{
    if (n < d_size) return;                   /* already computed */

    /* Grow the array if necessary. */
    if (d_cap < (size_t)(n + 1)) {
        size_t new_cap = d_cap ? d_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(d, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        d = tmp;
        d_cap = new_cap;
    }

    /* Initialise from scratch if array is empty. */
    if (d_size == 0) {
        d[0] = (seq_t)1;
        d_size = 1;
    }

    /* Compute the missing coefficients directly from the pentagonal theorem. */
    for (idx_t i = d_size; i <= n; ++i) {
        d[i] = (seq_t)0;                      /* default zero */

        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > i) break;                /* no more pentagonal numbers for this i */
            if (i == g1) {
                d[i] = (m & 1) ? (seq_t)(-1) : (seq_t)1;
                break;
            }
            idx_t g2 = m * (3 * m + 1) / 2;
            if (i == g2) {
                d[i] = (m & 1) ? (seq_t)(-1) : (seq_t)1;
                break;
            }
        }
    }

    d_size = n + 1;
}

/* Sequence a(n). */
seq_t a(idx_t n)
{
    ensure_d(n);                           /* make sure d[0..n] exists */

    seq_t sum = 0;
    for (idx_t t = 0; t <= n / 9; ++t)
        sum += d[n - 9 * t] * d[t];

    return sum;
}
