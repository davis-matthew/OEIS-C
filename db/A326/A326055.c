/*
 * A326055  a(n) = n - {the largest square that divides n}.
 * https://oeis.org/A326055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (largest square that divides n)
 * Range:     u8 n <= 256, u16 n <= 8988, u32 n <= 8988, u64 n <= 8988,
 *            u128 n <= 8988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Return a(n) for n >= 1.  The function is valid for all supported
   seq_t types and works for n up to 2^64. */
seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 0 */
        return 0;

    /* Factorise n by trial division.  n fits in a 64‑bit unsigned value. */
    uint64_t nn   = (uint64_t)n;   /* original value of n              */
    uint64_t rem  = nn;            /* number that will be divided      */
    seq_t    sq   = 1;             /* largest square divisor of n      */

    for (uint64_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            unsigned exp = 0;
            while (rem % p == 0) {
                rem  /= p;
                ++exp;
            }
            unsigned sq_exp = (exp / 2) * 2;      /* exponent in the square part */
            if (sq_exp) {                         /* p contributes to the square */
                seq_t mult = 1;
                for (unsigned i = 0; i < sq_exp; ++i)
                    mult *= (seq_t)p;
                sq *= mult;
            }
        }
    }
    /* Any remaining rem > 1 is a prime factor with exponent 1
       and therefore does not contribute to the square divisor. */

    return (seq_t)nn - sq;
}
