/*
 * A242219  Smallest a(n) in Pythagorean triple (a, b, c) such that c(n) -
 *          b(n) = n.
 * https://oeis.org/A242219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A242219: Smallest leg a(n) in a Pythagorean triple (a,b,c) with c(b) - b(n) = n
   Formula: using n = core(n) * f^2 with core(n) the square‑free kernel and
   f = A000188(n), x = f+1 if core(n) even, else f+2,
   a(n) = sqrt( n * ( n + (x² - f²) * core(n) ) )
   (c^2 - b^2 = n*(c+b) = a^2)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute core (square‑free kernel) and f (sqrt of largest square divisor) */
    idx_t temp = n;
    seq_t core = 1;
    seq_t f    = 1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 1)          /* odd exponent → include prime in core */
                core *= p;
            int half = exp >> 1;   /* floor(exp/2) for f */
            for (int i = 0; i < half; ++i)
                f *= p;
        }
    }
    if (temp > 1) {               /* remaining prime factor of exponent 1 */
        core *= temp;
        /* exp = 1 → f unchanged */
    }

    /* choose x */
    bool core_even = (core % 2 == 0);
    seq_t x = f + (core_even ? 1 : 2);

    /* compute a(n) */
    seq_t diff = x * x - f * f;
    seq_t t    = diff * core;
    seq_t sum  = (seq_t)n + t;
    seq_t prod = (seq_t)n * sum;
    return floor_sqrt(prod);
}
