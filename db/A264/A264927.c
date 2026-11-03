/*
 * A264927  Terms satisfy: a(2*n) = b(n)^2 and a(2*n+1) = b(n)*b(n+1) for n>=0
 *          with a(0)=1, where A(x)^2 = Sum_{n>=0} b(n)*x^n and g.f. A(x) =
 *          Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A264927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1 Let b(n) be the coefficients of A(x)^2. Then
 *            a(2*n) = b(n)^2 a(2*n+1) = b(n)*b(n+1) (for n ≥ 1) b(n) =
 *            Σ_{k=0}^n a(k)*a(n-k) (convolution of a with itself) The
 *            algorithm computes a(n) by first building the a sequence up to n
 *            and simultaneously computing b(n) as a convolution sum
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    idx_t N = n;
    idx_t bmax = N/2 + (N%2);          /* ceil(N/2)  */

    seq_t *a_arr = mem_new(seq_t, (N+1));
    seq_t *b_arr = mem_new(seq_t, (bmax+1));
    if (!a_arr || !b_arr) {
        /* allocation failed – return 0 as sentinel */
        if (a_arr) mem_free(a_arr);
        if (b_arr) mem_free(b_arr);
        return 0;
    }

    a_arr[0] = 1;
    a_arr[1] = 1;
    b_arr[0] = 1;                      /* b(0) = a(0)*a(0) = 1 */

    for (idx_t i = 1; i <= bmax; ++i) {
        seq_t s = 0;
        for (idx_t k = 0; k <= i; ++k) {
            s += a_arr[k] * a_arr[i - k];
        }
        b_arr[i] = s;

        idx_t idx_even = 2*i;
        if (idx_even <= N) {
            a_arr[idx_even] = b_arr[i] * b_arr[i];
        }

        if (i > 1) {                     /* compute odd a's except a(1) */
            idx_t idx_odd = 2*i - 1;
            if (idx_odd <= N) {
                a_arr[idx_odd] = b_arr[i-1] * b_arr[i];
            }
        }
    }

    seq_t result = a_arr[N];
    mem_free(a_arr);
    mem_free(b_arr);
    return result;
}
