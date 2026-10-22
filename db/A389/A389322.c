/*
 * A389322  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k-1,n-k).
 * https://oeis.org/A389322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes the a(n) term of OEIS sequence A389322.
 * n is the index (0‑based).  The function is correct for any
 * seq_t and idx_t types supplied by the build. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0U)
        return 1U;

    seq_t sum = 0U;
    seq_t bk   = 1U;          /* C(n,0) */

    for (idx_t k = 0U; k <= n; ++k) {
        /* bk is C(n,k) for this k */
        seq_t term1 = bk;

        /* Compute the second binomial: C(n+3*k-1 , n-k) */
        idx_t a = n + 3 * k - 1U;   /* upper index */
        idx_t b = n - k;           /* lower index */
        seq_t term2 = binomial(a, b);

        sum += term1 * term2;

        /* Prepare bk for the next k (k+1) */
        if (k < n) {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            bk = (bk * (uint64_t)(n - k)) / (uint64_t)(k + 1U);
        }
    }

    return sum;
}
