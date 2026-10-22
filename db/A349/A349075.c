/*
 * A349075  a(n) = U(n, 2*n), where U(n, x) is the Chebyshev polynomial of the
 *          second kind.
 * https://oeis.org/A349075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n, 2*n), where U is the Chebyshev polynomial of the
 *            second kind. Computed by the recurrence U_0(x) = 1 U_1(x) = 2x
 *            U_{k+1}(x) = 2x U_k(x) - U_{k-1}(x) with x = 2*n. The
 *            implementation protects the 64‑bit build from intermediate
 *            overflow by promoting the product to __int128 when sizeof(se...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t x = (seq_t)2 * (seq_t)n;   /* x = 2*n */

    if (n == 1) return (seq_t)2 * x; /* U_1 = 2x */

    seq_t u_prevprev = (seq_t)1;       /* U_0 */
    seq_t u_prev     = (seq_t)2 * x;   /* U_1 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t temp;
        if (sizeof(seq_t) <= 8) {
            /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds */
            temp = (seq_t)((unsigned __int128)2 * x * u_prev);
        } else {
            temp = (seq_t)2 * x * u_prev;
        }
        seq_t u_curr = temp - u_prevprev;
        u_prevprev = u_prev;
        u_prev = u_curr;
    }
    return u_prev;
}
