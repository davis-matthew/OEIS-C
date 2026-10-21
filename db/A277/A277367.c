/*
 * A277367  a(n) = gcd(A006666(n), A006667(n)) where A006666 and A006667 are
 *          respectively the number of halving and tripling steps in the
 *          '3x+1' problem.
 * https://oeis.org/A277367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A006666(n), A006667(n)) where A006666(n) = number of
 *            halving steps, and A006667(n) = number of tripling steps in the
 *            Collatz 3x+1 iteration. a(1) = 0 (both counts are 0)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the gcd of the halving and tripling counts for the Collatz
   sequence starting at n.  n is a positive integer (offset 1).  The
   function works for all n >= 1, with no precomputed table, and it
   is O(ok) per call, where *ok* is the length of the Collatz
   trajectory (at most a few hundred steps for n <= 10000).  The
   implementation uses 64‑bit arithmetic for the Collatz walk; for
   very large numerators the values are still well below 2^63, so
   storing them in idx_t (uint64_t) is safe.  The result is returned
   as a seq_t value, and the standard 'gcd' function from the utils
   library is used. */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Special case: n==1 gives 0 halving and 0 tripling steps. */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Use uint64_t for the Collatz walk; this is safe for
       starting n up to 10000 and the trajectory stays below 2^63. */
    uint64_t cur = (uint64_t)n;

    /* Counters for halving (even steps) and tripling (odd steps). */
    seq_t halving = 0;
    seq_t tripling = 0;

    while (cur != 1) {
        if ((cur & 1ULL) == 0ULL) {          /* even step: /2 */
            cur >>= 1;
            halving++;
        } else {                             /* odd step: 3*x+1 */
            cur = 3ULL * cur + 1ULL;
            tripling++;
        }
    }

    /* Return the greatest common divisor of the two counts. */
    return gcd(halving, tripling);
}
