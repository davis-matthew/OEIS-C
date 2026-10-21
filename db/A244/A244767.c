/*
 * A244767  Prime numbers ending in the prime number 29.
 * https://oeis.org/A244767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Primorial search: a(n) is the nth prime of the form 100*k+29.
   The function simply iterates k = 0,1,2,... and returns the nth
   candidate that passes the big-prime test is_prime_big.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset; return 0 as placeholder */
    idx_t found = 0;
    idx_t k = 0;
    seq_t result = 0;

    while (found < n) {
        seq_t cand = (seq_t)k * 100 + 29;   /* candidate of the form 100*k+29 */
        if (is_prime_big(cand)) {
            found++;
            if (found == n) {
                result = cand;
                break;
            }
        }
        k++;
    }
    return result;
}
