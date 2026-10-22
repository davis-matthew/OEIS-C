/*
 * A309274  Ackermann Coding (BIT predicate) of transitive hereditarily finite
 *          sets.
 * https://oeis.org/A309274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th nonnegative integer whose binary representation
 *            * satisfies: for every bit position k that is set, all bits set
 *            in the binary * representation of k are also set. a(1)=0
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/valid.h"

/* The helper functions listed in the requirements are not strictly needed
 * for this implementation.  The validate routine is written generically
 * so it works for any unsigned seq_t (uint8_t …, unsigned __int128,
 * or arbitrary‑precision integer classes). */

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) is not defined but return 0 */
        return (seq_t)0;

    idx_t count = 0;     /* number of valid integers found so far */
    seq_t candidate = 0; /* current integer being tested */

    /* Scan integers until the n‑th valid one is reached. */
    while (true) {
        if (valid(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
