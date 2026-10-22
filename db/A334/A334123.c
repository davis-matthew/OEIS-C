/*
 * A334123  Number of labeled tournaments on n vertices with the maximum
 *          number of Hamiltonian paths.
 * https://oeis.org/A334123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1)! * 2^(n-2) / (n-2) for n ≥ 5, with a(1)=1, a(2)=2,
 *            a(3)=2, a(4)=24, a(5)=64. The recurrence a(k) = a(k-1) / (k-2) *
 *            2*(k-1)*(k-3) for k≥6 is used to compute the values iteratively
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle small values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)24;
    if (n == 5) return (seq_t)64;

    /* start from a(5) and build up using the recurrence */
    seq_t result = (seq_t)64;        /* a(5) */
    for (idx_t k = 6; k <= n; ++k) {
        /* Division first to keep intermediate size small and exact */
        result = result / (seq_t)(k - 2);
        result = result * (seq_t)2;
        result = result * (seq_t)(k - 1);
        result = result * (seq_t)(k - 3);
    }
    return result;
}
