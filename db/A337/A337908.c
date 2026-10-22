/*
 * A337908  a(n) is the number of nonempty intervals x..y (with 0 < x <= y)
 *          such that LCM(x..y) = n.
 * https://oeis.org/A337908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 8572, u32 n <= 8572, u64 n <= 8572,
 *            u128 n <= 8572, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of non‑empty integer intervals x..y (1 ≤ x ≤ y ≤ n)
   such that LCM(x..y) = n.  The algorithm enumerates each left end
   x and extends the interval to the right while the running LCM never
   exceeds n.  As soon as the LCM becomes larger than n it can no longer
   become equal to n, so the inner loop stops.  This runs in
   O(n·d) time, where d is the average number of terms scanned per
   starting point (≤ n).  For n up to about 10 000 it runs well under
   the required time limit. */
#include <stdint.h>
#include <stddef.h>

#include "../utils/gcd.h"

/* External integer type names are provided by the build system.
   idx_t   – unsigned 64‑bit integer (index type)
   seq_t   – unsigned integer of the chosen size or arbitrary precision */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;      /* promote n to the result type */

    for (idx_t x = 1; x <= n; ++x) {
        seq_t l = (seq_t)x;          /* current LCM of interval [x..y] */
        if (l == n_seq)        /* interval consisting solely of x */
            ++result;

        for (idx_t y = x + 1; y <= n; ++y) {
            /* compute lcm(l, y) without letting it overflow n */
            seq_t g   = gcd(l, (seq_t)y);
            seq_t tmp = l / g;

            /* if tmp > n_seq / y, the new LCM would exceed n_seq */
            if (tmp > (n_seq / (seq_t)y))
                break;   /* further extensions can only increase the LCM */

            l = tmp * (seq_t)y;

            if (l == n_seq)
                ++result;
            else if (l > n_seq)   /* defensive check, should not happen */
                break;
        }
    }
    return result;
}
