/*
 * A365589  Numbers that have at least one prime digit and at least one
 *          nonprime digit.
 * https://oeis.org/A365589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   has_prime_and_nonprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose decimal representation contains at least one
   prime digit (2,3,5,7) and at least one nonprime digit (0,1,4,6,8,9). */
#include <stdbool.h>
#include "../utils/has_prime_and_nonprime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          // undefined behavior for n<1; return 0
    idx_t count = 0;
    idx_t cand   = 12;            // first number with both prime and nonprime digits
    while (1) {
        if (has_prime_and_nonprime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
