/*
 * A253298  Digital root for the following sequences, F(4*n)/F(4);
 *          F(12*n)/F(12); F(20*n)/F(20), where the pattern increases by 8, ad
 *          infinitum, with the Fibonacci numbers F = A000045.
 * https://oeis.org/A253298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is a periodic sequence with period 18 * a(n) = digital root
 *            of F(k*n)/F(k), * where k = 4 + 8*floor((n-1)/8). The sequence *
 *            repeats: 1,7,3,5,5,3,7,1,9,8,2,6,4,4,6,2,8,9
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of length 18 (index 0..17 correspond to n=1..18) */
    static const uint8_t period[18] = {
        1, 7, 3, 5, 5, 3, 7, 1, 9, 8,
        2, 6, 4, 4, 6, 2, 8, 9
    };
    idx_t r = (n - 1) % 18;
    return (seq_t)period[r];
}
