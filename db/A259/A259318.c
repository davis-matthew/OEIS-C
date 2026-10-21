/*
 * A259318  a(n) = A259109(n)*A006331(n) - A259108(n)^2.
 * https://oeis.org/A259318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (16*n^10+80*n^9+120*n^8-147*n^6-105*n^5+5*n^4+25*n^3+6*n^2)/525
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 119, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to keep the calculation exact for all
     * indices that fit in the target 64‑bit output.  The final result is
     * guaranteed to be an integer divisible by 525.                      */
    __uint128_t N  = ( __uint128_t ) n;        // cast once
    __uint128_t n1 = N;
    __uint128_t n2 = n1 * n1;                  // n^2
    __uint128_t n3 = n2 * n1;                  // n^3
    __uint128_t n4 = n3 * n1;                  // n^4
    __uint128_t n5 = n4 * n1;                  // n^5
    __uint128_t n6 = n5 * n1;                  // n^6
    __uint128_t n7 = n6 * n1;                  // n^7
    __uint128_t n8 = n7 * n1;                  // n^8
    __uint128_t n9 = n8 * n1;                  // n^9
    __uint128_t n10 = n9 * n1;                 // n^10

    __uint128_t num = 16 * n10
                    + 80 * n9
                    + 120 * n8
                    - 147 * n6
                    - 105 * n5
                    + 5  * n4
                    + 25 * n3
                    + 6  * n2;

    return (seq_t)(num / 525);
}
