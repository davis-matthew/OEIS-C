/*
 * A329269  Integers k such that 8*k + 1 is a prime or a square.
 * https://oeis.org/A329269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329269:
 * a(n) is the nth non‑negative integer k such that 8*k + 1 is prime or a perfect
 * square.  The code simply counts such k values by testing each 8*k+1 for
 * primality (using isPrimeGeneric) or squareness (using floor_sqrt). */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;;) {
        /* compute 8*k+1, using seq_t arithmetic to avoid overflow */
        seq_t m = ((seq_t)8 * (seq_t)k) + 1;

        /* check if m is a perfect square */
        seq_t s = floor_sqrt(m);
        if ((s * s == m) || isPrimeGeneric(m)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
