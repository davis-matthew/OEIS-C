/*
 * A337464  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of sqrt(2 / ( (1+2*(k-4)*x+((k+4)*x)^2)
 *          * (1-(k+4)*x+sqrt(1+2*(k-4)*x+((k+4)*x)^2)) )).
 * https://oeis.org/A337464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 34, i32 n <= 131, i64 n <= 369,
 *            i128 n <= 1066, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A337464
    Computed via the antidiagonal reading of the square array T(n,k)
    defined by:
          T(n,k) = Σ_{j=0..n} (-k)^(n‑j) * binomial(2*j,j) * binomial(2*n+1,2*j)
    with T(0,k)=1.  For a given index n, find the diagonal d and row i
    such that n = d(d+1)/2 + i (0 ≤ i ≤ d), then set k = d – i and evaluate
    the above sum.  Special‐case k==0 (only the j==n term survives).  */
/* Uses binomial from ../utils/binomial.h */

#include "../utils/binomial.h"

/* external linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* Find diagonal d and row i for index n.
       cum is the number of terms before diagonal d (i.e. d*(d+1)/2). */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum + (d + 1) <= n) {
        cum += d + 1;
        ++d;
    }
    idx_t i = n - cum;            /* row index (0 ≤ i ≤ d) */
    idx_t k = d - i;              /* column index for T(i,k) */

    /* Special case k == 0: only j == i contributes. */
    if (k == 0) {
        seq_t bi = binomial(2 * i, i);          /* C(2i,i) */
        idx_t t   = 2 * i + 1;                  /* 2i+1 */
        return bi * (seq_t)t;
    }

    /* General case: compute Σ_{j=0..i} (-k)^(i‑j) * C(2j,j) * C(2i+1,2j). */
    seq_t base = -(seq_t)k;   /* (-k) as seq_t */
    seq_t pow  = 1;           /* (-k)^i (will be divided by base each step) */

    /* Compute (-k)^i first */
    for (idx_t e = 1; e <= i; ++e) {
        pow *= base;
    }

    seq_t result = 0;
    for (idx_t j = 0; j <= i; ++j) {
        seq_t term = pow;
        term *= binomial(2 * j, j);
        term *= binomial(2 * i + 1, 2 * j);
        result += term;
        /* Reduce exponent for next j: multiply by 1/(-k) => divide by base */
        pow /= base;
    }
    return result;
}
