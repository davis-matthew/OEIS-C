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
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x satisfies the closure property described above. */
static bool valid(seq_t x)
{
    /* Fast path for zero. */
    if (x == (seq_t)0)
        return true;

    /* Iterate over the binary digits of x from least significant to most. */
    unsigned pos = 0;          /* current bit position k */
    seq_t   trail = x;         /* remaining bits to examine */

    while (trail) {
        if (trail & (seq_t)1) {
            /* For a set bit at position pos (k = pos), all bits set in
             * pos (i.e. the binary representation of pos) must also be set
             * in x.  The mask of bits set in pos is simply the integer value
             * of pos itself. */
            seq_t mask = (seq_t)pos;
            if ((x & mask) != mask)
                return false;
        }
        trail >>= 1;
        ++pos;
    }
    return true;
}

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
