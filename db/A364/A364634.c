/*
 * A364634  a(n) = n * LegendreP(n, 3).
 * https://oeis.org/A364634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * LegendreP(n,3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth Legendre polynomial at x = 3 via the
   recurrence (k+1)P_{k+1} = (2k+1)xP_k - kP_{k-1},
   then returns n * P_n(3). */
/* C11 / C++17 compatible. Uses dynamic allocation that is
   safe for both primitive integer seq_t and arbitrary‑precision
   integer classes. */

#ifdef __cplusplus
#  include <cstddef>
#else
#  include <stddef.h>
#  include <stdlib.h>
#endif

/* Helper to allocate/free an array of seq_t in a way that works for
   both C and C++. */
static seq_t *allocate_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* static cache for Legendre polynomials at x = 3 */
    static seq_t *P = NULL;        /* array of P_k(3) */
    static size_t len = 0;         /* number of elements stored */
    static size_t cap = 0;         /* allocated capacity */

    /* ensure capacity for indices up to n */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? (cap * 2) : 2;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = allocate_seq_array(new_cap);
        /* copy existing values if any */
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = P[i];
        free_seq_array(P);
        P   = tmp;
        cap = new_cap;
    }

    /* initialise the first two polynomials if not yet done */
    if (len == 0) {
        P[0] = (seq_t)1;          /* P_0(x) = 1 */
        len  = 1;
    }
    if (len == 1) {
        P[1] = (seq_t)3;          /* P_1(3) = 3 */
        len  = 2;
    }

    /* compute missing values up to n */
    for (size_t k = len; k <= (size_t)n; ++k) {
        /* for k >= 2:
           P_k = ((2k-1)*x*P_{k-1} - (k-1)*P_{k-2}) / k
           with x = 3 */
        seq_t term1 = (seq_t)(2 * k - 1) * (seq_t)3 * P[k - 1];
        seq_t term2 = (seq_t)(k - 1) * P[k - 2];
        seq_t num   = term1 - term2;
        P[k]        = num / (seq_t)k;      /* division is exact */
    }
    len = (size_t)n + 1;

    /* finally return a(n) = n * P_n(3) */
    return (seq_t)n * P[n];
}
