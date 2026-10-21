/*
 * A247683  Odd composite numbers congruent to 7 modulo 9.
 * https://oeis.org/A247683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1843, u32 n <= 3209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd composite numbers congruent to 7 mod 9.
   This routine generates candidates of the form 9*k + 7,
   checks that they are odd and composite using the
    is_prime_big helper, and returns the nth such value. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 0;
    seq_t cand;

    while (1) {
        cand = (seq_t)k * 9 + 7;          /* numbers of the form 9k+7 */

        if ((cand & 1) && !is_prime_big(cand)) { /* odd and composite */
            ++found;
            if (found == n)
                return cand;
        }
        ++k;
    }
}
