/*
 * A293162  Take every third term of the {0,1}-version of the Thue-Morse
 *          sequence: a(n) = A010060(3n).
 * https://oeis.org/A293162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(3*n) mod 2
 * Range:     u8 n <= 9277, u16 n <= 9277, u32 n <= 9277, u64 n <= 9277,
 *            u128 n <= 9277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for the multiplication – n is an
       idx_t which is uint64_t, and 3*n fits in a uint64_t for the
       input sizes considered here.  The parity is computed in
       64‑bit arithmetic and then cast back to seq_t. */
    uint64_t m = (uint64_t)n * 3U;
    uint64_t p = 0;

    while (m) {
        p ^= m & 1U;
        m >>= 1;
    }
    return (seq_t)p;
}
