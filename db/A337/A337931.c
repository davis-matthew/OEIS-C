/*
 * A337931  Number of ways to write n as the sum of two positive integers, s,t
 *          such that phi(s) is not equal to phi(t), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A337931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..⌊n/2⌋} (1 - [phi(i) = phi(n-i)])
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;           /* floor(n/2) */
    unsigned long long count = 0;  /* use a wider type to avoid overflow */
    for (idx_t i = 1; i <= half; ++i) {
        seq_t phi_i = phi(i);
        seq_t phi_j = phi(n - i);
        if (phi_i != phi_j) {
            count++;
        }
    }
    return (seq_t)count;
}
