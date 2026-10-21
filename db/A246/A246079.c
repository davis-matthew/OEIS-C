/*
 * A246079  Paradigm shift sequence for (-1,5) production scheme with
 *          replacement.
 * https://oeis.org/A246079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j≥0} 3^j · c_{n-1-14j} where c_i are the coefficients
 *            of the polynomial 1 + 2x + 3x² + … + x³⁰ + 2x³¹ (equivalently
 *            a(n)=3*a(n‑14) for all n ≥ 33)
 * Range:     u8 n <= 51, u16 n <= 121, u32 n <= 263, u64 n <= 545,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Coefficients of the numerator polynomial, indices 0 .. 31 */
static const uint8_t coeff[32] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9,10,
   11,12,13,14,12,10, 8, 6, 5, 4,
    3, 2, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 2
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, return 0 */
    seq_t result = 0;
    seq_t pow3   = 1;                     /* 3^j */
    int64_t pos  = (int64_t)n - 1;         /* 0‑based position of the coefficient */

    while (pos >= 0) {
        if (pos < 32) {
            result += pow3 * (seq_t)coeff[(size_t)pos];
        }
        pos -= 14;
        pow3 *= 3;
    }

    return result;
}
