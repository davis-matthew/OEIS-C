/*
 * A352501  a(n) is the distance from n to the nearest integer that can be
 *          added to n without carries in balanced ternary.
 * https://oeis.org/A352501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 534, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A352501: a(n) = distance from n
   to the nearest integer k that can be added to n without carries
   in balanced ternary.  The value is found by a brute‑force search
   of increasing distance d until a compatible integer is found.
   Since for any n there is always a compatible integer (k = 0),
   the algorithm terminates.  Because a(n) grows very slowly
   (at most a few dozens for n ≤ 10⁵), this simple method
   is well within the time limit.  */

#include <stdbool.h>

bool compatible(idx_t n, idx_t m)
{
    idx_t a = n;
    idx_t b = m;

    while (a > 0 || b > 0) {
        int da, db;

        /* digit of a */
        if (a == 0) {
            da = 0;
        } else {
            int r = a % 3;
            if (r == 0) {
                da = 0;
                a /= 3;
            } else if (r == 1) {
                da = 1;
                a /= 3;
            } else {            /* r == 2 -> digit -1, carry */
                da = -1;
                a = (a + 1) / 3;
            }
        }

        /* digit of b */
        if (b == 0) {
            db = 0;
        } else {
            int r = b % 3;
            if (r == 0) {
                db = 0;
                b /= 3;
            } else if (r == 1) {
                db = 1;
                b /= 3;
            } else {            /* r == 2 -> digit -1, carry */
                db = -1;
                b = (b + 1) / 3;
            }
        }

        if ((da == 1 && db == 1) || (da == -1 && db == -1))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Search increasing distance until a compatible integer is found. */
    for (idx_t d = 1;; ++d) {
        /* check n - d, if it does not underflow */
        if (d <= n) {
            if (compatible(n, n - d))
                return (seq_t)d;
        }
        /* check n + d */
        if (compatible(n, n + d))
            return (seq_t)d;
        /* guarantee termination: the farthest possible distance is n
           (n + n = 2n compatible with 0).  */
    }
}
