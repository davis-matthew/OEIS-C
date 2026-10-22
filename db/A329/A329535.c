/*
 * A329535  Numbers with twice as many halving steps before reaching 1 in the
 *          3x + 1 problem as tripling steps.
 * https://oeis.org/A329535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A329535 by brute‑force Collatz counts. */
#include <stdbool.h>

/* Return true iff m has twice as many halving steps as tripling steps
   in its Collatz trajectory until it first reaches 1.  */
static bool satisfies(seq_t m) {
    unsigned __int128 cur = (unsigned __int128)m;
    idx_t halving = 0, tripling = 0;
    while (cur != 1) {
        if ((cur & 1) == 0) {        /* even -> halve */
            cur >>= 1;
            halving++;
        } else {                     /* odd -> 3x + 1 */
            cur = cur * 3 + 1;
            tripling++;
        }
    }
    return halving == 2 * tripling;
}

/* a(n) = the nth natural number m (in increasing order) such that
   the number of halving steps in its Collatz trajectory until the
   first visit to 1 is twice the number of tripling steps.   */
seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t m = 1;
    for (;;) {
        if (satisfies(m)) {
            found++;
            if (found == n) return m;
        }
        m++;
    }
}
