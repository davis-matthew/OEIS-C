/*
 * A349073  a(n) = U(2*n, n), where U(n, x) is the Chebyshev polynomial of the
 *          second kind.
 * https://oeis.org/A349073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(2*n, n) (Chebyshev polynomial of the second kind)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* For n = 0, U(0,0) = 1. For n = 1, U(2,1) = 3. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* two_n = 2*n; will fit in idx_t for the expected n range (≤ 10000). */
    idx_t steps = (idx_t)((seq_t)2 * (seq_t)n);  /* number of iterations = 2*n */

    seq_t prev = (seq_t)1;          /* U_0 = 1 */
    seq_t curr = (seq_t)2 * (seq_t)n;  /* U_1 = 2*n */

    for (idx_t i = 2; i <= steps; ++i) {
        seq_t next = (seq_t)2 * (seq_t)n * curr - prev;  /* U_k = 2*n*U_{k-1} - U_{k-2} */
        prev = curr;
        curr = next;
    }

    return curr;  /* U_{2*n} */
}
