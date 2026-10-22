/*
 * A352895  The maximum binary weight of those elements of the Collatz orbit
 *          of n that follow after the term n itself, when iterated down to 1,
 *          or -1 if 1 is never reached.
 * https://oeis.org/A352895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum binary weight of the terms that occur in the
 *            Collatz orbit of n after the element n itself, iterated until 1
 *            is reached. If the orbit never reaches 1 the function returns
 *            (seq_t)-1
 * Range:     u8 n <= 8414, u16 n <= 8414, u32 n <= 8414, u64 n <= 8414,
 *            u128 n <= 8414, big any n
 * Helpers:   popcount_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount_uint128.h"

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Use a 128‑bit accumulator to avoid overflow on 3x+1 */
    unsigned __int128 cur = (unsigned __int128)n;
    seq_t max_weight = (seq_t)0;
    const unsigned max_iter = 1000000;          /* safety limit    */

    for (unsigned i = 0; i < max_iter; ++i) {
        /* Collatz step */
        if ((cur & 1) == 0) {
            cur >>= 1;
        } else {
            cur = cur * 3 + 1;
        }

        /* Update maximum binary weight */
        unsigned w = popcount_uint128(cur);
        if (w > max_weight) {
            max_weight = (seq_t)w;
        }

        if (cur == 1) {
            return max_weight;                 /* orbit finished */
        }
    }
    /* If the iteration limit is exceeded return -1 */
    return (seq_t)-1;
}
