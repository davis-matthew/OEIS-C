/*
 * A377463  Numbers that are not the sum of distinct powers of 4.
 * https://oeis.org/A377463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by scanning integers and selecting those whose base‑4
   digits contain 2 or 3.  See OEIS A377463 ("Numbers that are not the
   sum of distinct powers of 4"). */

#include <stdbool.h>

/* Return the n‑th term of the sequence (n ≥ 1).  The routine
   simply iterates over consecutive integers, keeps those with
   a base‑4 digit equal to 2 or 3, and stops when the n‑th such
   integer has been found.  No caching is required. */
seq_t a(idx_t n)
{
    idx_t  seen = 0;   /* How many qualifying numbers we have found */
    seq_t  m   = 1;    /* Current candidate integer */

    while (1) {
        seq_t t = m;
        bool has23 = false;

        /* Check each base‑4 digit of m */
        while (t > 0) {
            seq_t d = t % 4;
            if (d >= 2) {          /* digit is 2 or 3 */
                has23 = true;
                break;
            }
            t /= 4;
        }

        if (has23) {
            ++seen;
            if (seen == n)
                return m;
        }

        ++m;
    }
}
