/*
 * A294659  Largest number in the orbit of n under iteration of the map
 *          A293975: x -> x/2 if even, x + nextprime(x) else.
 * https://oeis.org/A294659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest value in the orbit of the number (n+1) under the
 *            map f(x) = x/2 if x is even, otherwise x + nextprime(x). The
 *            orbit is followed until the value 1 is seen twice (i.e. the
 *            cycle 1 → 3 → 8 → 4 → 2 → 1 is reached), at which point the
 *            maximum encountered value is returned. a(0) = 0
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* a(0) = 0 */

    seq_t cur = (seq_t)(n + 1);            /* start from n+1 */
    seq_t mx   = cur;
    bool seen_one = false;

    while (1) {
        if ((cur & 1) == 0) {                /* even: divide by 2 */
            cur >>= 1;
        } else {                            /* odd: add next prime */
            uint64_t cp = (uint64_t)cur;  /* cur fits in 64 bits for given bounds */
            uint64_t np = next_prime(cp);
            cur = cur + (seq_t)np;
        }

        if (cur > mx) mx = cur;

        if (cur == 1) {
            if (seen_one) break;            /* cycle completed */
            seen_one = true;                /* first visit of 1 */
        }
    }

    return mx;
}
