/*
 * A382629  Triangle T(n,k), n >= 0, 0 <= k <= n, read by rows, where T(n,k) =
 *          (n-k)*T(n-1,k-1) + 2*(k+1)*T(n-1,k) + A102365(n,k) with T(n,k) = 0
 *          if k < 0 or k > n.
 * https://oeis.org/A382629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (n-k)*T(n-1,k-1) + 2*(k+1)*T(n-1,k) + A102365(n,k),
 *            T(0,0)=1, T(n,k)=0 if k<0 or k>n; A102365(n,k) =
 *            (n-k)*A102365(n-1,k-1) + (2*k+1)*A102365(n-1,k), A102365(0,0)=1;
 *            a(n) is the triangle read by rows
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 69, u64 n <= 194,
 *            u128 n <= 506, big any n
 * Validated: standard, pass, 71 terms through n = 9779 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* find row r and column c with n = r(r+1)/2 + c */
    idx_t r = 0;
    idx_t rem = n;
    while (rem >= r + 1) {
        rem -= r + 1;
        r++;
    }
    idx_t c = rem;

    seq_t *T = mem_new(seq_t, r + 2);
    seq_t *A = mem_new(seq_t, r + 2);
    T[0] = 1;
    A[0] = 1;
    for (idx_t i = 1; i <= r; i++) {
        for (idx_t k = i + 1; k-- > 0; ) {
            seq_t an = (seq_t)(2 * k + 1) * A[k];
            seq_t tn = (seq_t)(2 * (k + 1)) * T[k];
            if (k > 0) {
                an += (seq_t)(i - k) * A[k - 1];
                tn += (seq_t)(i - k) * T[k - 1];
            }
            A[k] = an;
            T[k] = tn + an;
        }
    }
    seq_t res = T[c];
    mem_free(T);
    mem_free(A);
    return res;
}
