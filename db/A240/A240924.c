/*
 * A240924  Digital root of squares of numbers not divisible by 2, 3 or 5.
 * https://oeis.org/A240924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital root of the square of the nth integer not divisible by 2, 3, or 5.
   The numbers not divisible by 2, 3, or 5 repeat modulo 30 with residues
   {1,7,11,13,17,19,23,29}.  Since the digital root depends only on the
   value modulo 9, we compute (x % 9) where x is the nth such number,
   square it modulo 9, and map 0 to 9. */

/* Requires: idx_t and seq_t are provided by the build environment */
#include <stdint.h>

static const uint8_t residues[8] = {1,7,11,13,17,19,23,29};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset (not used) */

    /* nth valid number: x = 30 * floor((n-1)/8) + residue */
    idx_t m = n - 1;
    idx_t q = m >> 3;          /* floor((n-1)/8)          */
    idx_t t = m & 7;           /* (n-1) mod 8             */
    uint8_t r = residues[t];   /* residue modulo 30       */

    /* Compute x modulo 9 without forming x explicitly. */
    int qm = (int)(q % 3);     /* because 3*q mod 9 depends only on q mod 3 */
    int r_mod9 = r % 9;
    int x_mod9 = (3 * qm + r_mod9) % 9;

    /* Digital root of x^2 is (x^2 % 9), replace 0 with 9. */
    int sq_mod9 = (x_mod9 * x_mod9) % 9;
    return (sq_mod9 == 0) ? 9 : sq_mod9;
}
