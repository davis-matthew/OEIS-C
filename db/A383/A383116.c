/*
 * A383116  Permissible phase values, in degrees, associated with restricted
 *          Fourier amplitudes obtained via Fourier transform of a
 *          three-dimensional crystal lattice.
 * https://oeis.org/A383116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 15 * ( 6*floor((n-1)/4) + d ) where d = {0,2,3,4} for
 *            (n-1) mod 4 = 0,1,2,3 respectively
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* n ≥ 1; use idx_t for all intermediate arithmetic */
    idx_t cycles = (n - 1) / 4;
    idx_t rem    = (n - 1) % 4;
    idx_t base   = 6 * cycles;
    idx_t k;
    if (rem == 0) {
        k = base;
    } else if (rem == 1) {
        k = base + 2;
    } else if (rem == 2) {
        k = base + 3;
    } else { /* rem == 3 */
        k = base + 4;
    }
    return (seq_t)15 * (seq_t)k;
}
