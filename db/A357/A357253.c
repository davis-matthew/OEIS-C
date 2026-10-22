/*
 * A357253  a(n) is the largest prime < 6*n.
 * https://oeis.org/A357253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime <= 6*n (with offset 1)
 * Range:     u8 n <= 42, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Since n >= 1, 6*n >= 6.  The largest prime <= 6*n is always >= 2. */
    seq_t limit = (seq_t)n * 6;

    for (seq_t x = limit; x >= 2; --x) {
        if (is_prime_big(x))
            return x;
    }
    /* The function is defined only for n >= 1, so this point should never
       be reached. Return 0 as a safe fallback. */
    return 0;
}
