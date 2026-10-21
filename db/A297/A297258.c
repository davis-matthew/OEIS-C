/*
 * A297258  Numbers whose base-6 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_greater_down_variation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: base‑6 numbers whose down‑variation exceeds up‑variation.
   a(n) is the nth positive integer satisfying DV(n,6) > UV(n,6).  */

/* Requires <stdbool.h>, <stdlib.h>, <stdint.h> */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_greater_down_variation.h"

/* Main function: returns the nth term of A297258. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;

    /* Search all positive integers until we find the nth qualifying one. */
    for (seq_t x = 1; /* no upper bound */; ++x) {
        if (has_greater_down_variation(x)) {
            ++count;
            if (count == n) return x;
        }
    }
}
