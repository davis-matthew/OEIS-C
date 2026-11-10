/*
 * A376462  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k, k)*A108625(n,
 *          n-k).
 * https://oeis.org/A376462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2*binomial(n+k,k)*T(n,n-k),
 *            where T(n,m) = A108625(n,n-k) = Sum_{i=0..m}
 *            binomial(n,i)^2*binomial(n+m-i,m-i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m, j, i, k;
    seq_t *row = mem_new(seq_t, 2 * n + 2);
    seq_t *A = mem_new(seq_t, n + 1);   /* binomial(n,i)^2 */
    seq_t *B = mem_new(seq_t, n + 1);   /* binomial(n+j,j) */
    seq_t result = 0;

    row[0] = 1;
    for (m = 0; m <= 2 * n; m++) {
        if (m > 0) {
            for (j = m; j >= 1; j--)
                row[j] = row[j] + row[j - 1];
        }
        if (m == n) {
            for (i = 0; i <= n; i++) A[i] = row[i] * row[i];
        }
        if (m >= n) {
            j = m - n;
            B[j] = row[j];
        }
    }

    for (k = 0; k <= n; k++) {
        idx_t mm = n - k;
        seq_t t = 0;
        for (i = 0; i <= mm; i++)
            t += A[i] * B[mm - i];
        result += A[k] * B[k] * t;
    }

    mem_free(row);
    mem_free(A);
    mem_free(B);
    return result;
}
