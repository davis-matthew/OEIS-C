/*
 * A344872  Semiprimes of the form 3m+2.
 * https://oeis.org/A344872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth semiprime that is congruent to 2 modulo 3
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 2;               /* start searching from 2 */
    while (found < n) {
        if (x % (seq_t)3 == (seq_t)2) {
            if (count_prime_factors(x) == (seq_t)2) {
                ++found;
                if (found == n) return x;
            }
        }
        ++x;
    }
    return (seq_t)0; /* should never reach here for n >= 1 */
}
