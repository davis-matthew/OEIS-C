/*
 * A359762  Array read by ascending antidiagonals. T(n, k) = n!*[x^n] exp(x +
 *          (k/2) * x^2). A generalization of the number of involutions (or of
 *          'telephone numbers').
 * https://oeis.org/A359762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: T(n, k) = Σ_{j even ≤ n} binomial(n, j) * (j‑1)!! * k^{j/2}
   The sequence a(n) is the array of T(n, k) read by ascending antidiagonals:
     a(C(m) + i) = T(m-i, i)   for 0 ≤ i ≤ m         (C(m) = m(m+1)/2)
   Where C(m) = m(m+1)/2 is the number of entries before diagonal m. */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */
#include <stdint.h>

/* Compute T(n, k) using the summation formula. */
static seq_t
T_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t kseq = (seq_t)k;   /* convert k to seq_t once */

    /* j = 0 term */
    sum += binomial(n, 0);   /* binomial(n, 0) == 1 */

    /* For even j >= 2 update double factorial and k^(j/2) incrementally. */
    seq_t dbl = 1;   /* (j‑1)!! for current even j, starts with j=0 → dbl = 1 */
    seq_t powk = 1;  /* k^(j/2), starts with j=0 → powk = 1 */

    for (idx_t j = 2; j <= n; j += 2) {
        /* (j‑1)!! = (j‑1) * (j‑3)!!; multiply by the odd number (j‑1). */
        dbl *= (seq_t)(j - 1);

        /* k^(j/2) = k * k^((j‑2)/2) ; multiply by k. */
        powk *= kseq;

        sum += binomial(n, j) * dbl * powk;
    }

    return sum;
}

/* Return the n-th term of the flattened array A359762. */
seq_t
a(idx_t N)
{
    /* Find the diagonal index m such that
       C(m) = m*(m+1)/2 ≤ N < C(m+1) = (m+1)*(m+2)/2          */
    idx_t m = 0;
    /* Use 128‑bit arithmetic to avoid overflow for very large N. */
    while (((unsigned __int128)(m + 1) * (m + 2) / 2) <= N)
        ++m;

    /* Number of entries before diagonal m. */
    idx_t prev = (unsigned __int128)m * (m + 1) / 2;

    /* Position inside the current diagonal. */
    idx_t p = N - prev;          /* 0 ≤ p ≤ m */

    /* Mapping inside diagonal:
          (n, k) = (m‑p, p)          */
    idx_t n = m - p;
    idx_t k = p;

    return T_value(n, k);
}
