/*
 * A372826  Exponents of 3 and 2 in sequence A372824.
 * https://oeis.org/A372826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = n, a(2n+1) = greatest k such that 2^k < 3^(n+1) (i.e.
 *            floor(log₂(3^(n+1))) )
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   bit_length, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1UL) == 0) {
        return (seq_t)(n >> 1);
    }

    /* Odd indices: n = 2*half + 1 */
    idx_t half = (n - 1) >> 1;          /* n = 2*half + 1 */

    /* Compute 3^(half+1) */
    seq_t pow3_val = pow3(half + 1);

    /* floor(log₂(pow3_val)) = bitlen_seq_t(pow3_val) - 1 */
    unsigned int bl = bit_length(pow3_val);

    return (seq_t)(bl - 1);
}
