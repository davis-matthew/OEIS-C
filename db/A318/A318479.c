/*
 * A318479  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          h(n) = Sum_{k=0..w} b_k * (i-1)^k (where i denotes the imaginary
 *          unit); a(n) is the square of the modulus of h(n).
 * https://oeis.org/A318479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = | Σ_{k:b_k=1} (i-1)^k |^2, with i^2 = -1 Computed by
 *            summing the real and imaginary parts of (i-1)^k for each set bit
 *            k in the binary representation of n. Integers k are processed
 *            from least to most significant bit. The magnitude squared is
 *            returned as seq_t
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Result will always be non‑negative.  Use signed 64‑bit
       intermediate variables – their magnitude stays well below
       2^63 for the intended usage range (n ≤ ~10^4). */
    int64_t real = 0, imag = 0;   /* accumulated real/imag parts of h(n) */
    int64_t zr   = 1, zi = 0;     /* (i-1)^k for current k, starting at k=0 */

    /* Process each bit of n */
    while (n != 0)
    {
        if (n & 1) {
            real += zr;
            imag += zi;
        }

        /* Prepare next power:  (a+bi)*(−1+i) = (−a−b)+(a−b)i */
        int64_t next_zr = -zr - zi;
        int64_t next_zi =  zr - zi;
        zr = next_zr;
        zi = next_zi;

        n >>= 1;
    }

    /* Compute |h(n)|^2 = real^2 + imag^2 and convert to seq_t */
    /* The product fits in int64_t for the range of n considered */
    int64_t square = real * real + imag * imag;
    return (seq_t) square;
}
