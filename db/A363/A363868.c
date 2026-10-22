/*
 * A363868  a(n) = A108625(3*n, n).
 * https://oeis.org/A363868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} binomial(3*n, n-k)^2 * binomial(3*n+k, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here we compute the terms directly for every n.  For each n we
   maintain a running binomial C = binomial(3*n, n-k) and a running
   binomial B = binomial(3*n+k, k).  Both are updated with an
   exact division that never loses precision. */

#include <stddef.h>

/* Compute binomial(n,k) multiplicatively.  Handles n,k up to 64‑bit. */
static seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;               /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);     /* numerator */
        res = res / (seq_t)i;               /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 */

    seq_t sum = 0;
    /* initial binomial C = binom(3*n, n) */
    seq_t C = binom_nk(3 * n, n);
    /* initial B = binom(3*n+0, 0) = 1 */
    seq_t B = 1;

    /* k = 0 term */
    seq_t term = C * C;
    term *= B;
    sum += term;

    for (idx_t k = 1; k <= n; ++k) {
        /* update C to binom(3*n, n-k) */
        idx_t t = n - (k - 1);                             /* current t = n-k+1 */
        C = C * (seq_t)t / (seq_t)(3 * n - t + 1);

        /* update B to binom(3*n + k, k) */
        B = B * (seq_t)(3 * n + k) / (seq_t)k;

        /* add term */
        term = C * C;
        term *= B;
        sum += term;
    }

    return sum;
}
