/*
 * A397605  a(n) = Sum_{k=0..n} n^(n - k)*hypergeom([k - 1, k - n], [], -1/n)
 *          for n > 0, a(0) = 1. Row sums of A397647.
 * https://oeis.org/A397605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * hypergeom([k-1, k-n], [], -1/n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum using integer arithmetic only. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Pre‑compute powers of n: pow[e] = n^e, 0 <= e <= n */
    seq_t *pow = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    pow[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow[i] = pow[i - 1] * (seq_t)n;

    seq_t result = 0;

    /* Skip k = 0 because H(0) = 0 for all n. */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxm = n - k;
        seq_t A = 1;      /* A = C(k+m-2, m) */
        seq_t B = 1;      /* B = (n-k)! / (n-k-m)! */

        for (idx_t m = 0; m <= maxm; ++m) {
            /* term = A * B * n^(n-k-m) */
            seq_t term = A * B;
            term = term * pow[n - k - m];
            result += term;

            if (m == maxm)
                break;

            /* Update A to C(k+m-1, m+1) = A * (k+m-1)/(m+1) */
            seq_t multA = (seq_t)(k + m - 1);
            A = A * multA;
            A = A / (seq_t)(m + 1);

            /* Update B to (n-k)!/(n-k-(m+1))! = B * (n-k-m) */
            B = B * (seq_t)(n - k - m);
        }
    }

    free(pow);
    return result;
}
