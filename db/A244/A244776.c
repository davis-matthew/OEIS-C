/*
 * A244776  Prime numbers ending in the prime number 83.
 * https://oeis.org/A244776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime of the form 100*k + 83 (k ≥ 0),  offset 1 */
/* Uses the helper bool is_prime(idx_t n) from ../utils/is_prime.h */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(1) is 83; for n <= 0 the function is outside the offset and
       returns 0, which is acceptable. */
    if (n == 0) {
        return (seq_t)0;
    }

    uint64_t candidate = 83ULL;   /* 100*0 + 83 */
    idx_t count = 0;

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                /* Truncate to seq_t; correct when value fits in seq_t. */
                return (seq_t)candidate;
            }
        }
        candidate += 100ULL;   /* Next number of the form 100*k + 83 */
    }
}
