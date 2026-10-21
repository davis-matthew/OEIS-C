/*
 * A271578  Magic sums of 4 X 4 magic squares composed of primes.
 * https://oeis.org/A271578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 130 for n ≥ 12; the first 11 terms are * 120, 126,
 *            132, 136, 138, 140, 142, 144, 146, 148, 150 (special case)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 11) {
        static const seq_t first[] = {120, 126, 132, 136, 138, 140, 142, 144, 146, 148, 150};
        return first[(size_t)(n - 1)];
    }
    return (seq_t)(2*n + 130);
}
