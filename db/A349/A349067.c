/*
 * A349067  a(n) = H(3*n, n), where H(n,x) is n-th Hermite polynomial.
 * https://oeis.org/A349067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = H(3*n, n) -- the physicists' (probabilists') Hermite
 *            polynomial
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*
 * Compute the Hermite polynomial H_m(x) using the recurrence
 *
 *   H_0(x) = 1
 *   H_1(x) = 2*x
 *   H_{k+1}(x) = 2*x*H_k(x) - 2*k*H_{k-1}(x)
 *
 * Here m = 3*n and x = n.
 * The function works for any integer seq_t provided by the build, be it plain
 * signed integer or arbitrary‑precision class, and is O(m) in time and O(1)
 * additional space.
 */
seq_t a(idx_t n)
{
    /* Special case n = 0 : m = 0, H_0(x) = 1 */
    if (n == 0)
        return (seq_t)1;

    const idx_t m = (idx_t)3 * n;          /* degree of the Hermite polynomial */
    const seq_t x = (seq_t)n;              /* evaluation point */

    /* Base values H_0 and H_1 */
    seq_t H_prev = (seq_t)1;              /* H_0 */
    seq_t H_curr = (seq_t)2 * x;          /* H_1 */

    /* Iterate the recurrence until we reach H_m */
    for (idx_t k = 1; k < m; ++k)
    {
        /* H_{k+1} = 2*x*H_k - 2*k*H_{k-1} */
        seq_t H_next = ((seq_t)2 * x * H_curr) - ((seq_t)2 * (seq_t)k * H_prev);
        H_prev = H_curr;
        H_curr = H_next;
    }

    /* After the loop, H_curr holds H_m = H(3*n, n) */
    return H_curr;
}
