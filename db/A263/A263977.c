/*
 * A263977  Integers k > 0 such that k^2 + p^2 is prime for some prime p.
 * https://oeis.org/A263977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263977: integers k such that k² + p² is prime for some prime p.
   An odd k is a member iff k² + 4 is prime; every even k is a member.
   The function scans successive k, counting members until the n-th is
   reached.  It uses the provided is_prime_big() routine for primality
   testing of the value k²+4. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;

    for (;;) {
        if ((k & 1ULL) == 0ULL) {           /* even k always a member */
            ++count;
            if (count == n)
                return (seq_t)k;
        } else {                            /* odd k: test k²+4 prime */
            uint64_t temp = (uint64_t)k * k + 4ULL;
            seq_t val = (seq_t)temp;
            if (is_prime_big(val)) {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }
}
