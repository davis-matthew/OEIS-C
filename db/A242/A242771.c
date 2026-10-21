/*
 * A242771  Number of integer points in a certain quadrilateral scaled by a
 *          factor of n (another version).
 * https://oeis.org/A242771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived formula for OEIS A242771:
   a(1) = 0.
   For n ≥ 2:
     let d(1) = 0
     d(k+1) = d(k) + p[(k-1) mod 6],   k ≥ 1
     where p = [1, 1, 1, 0, 2, 0]
     Then a(n) = a(n-1) + d(n-1)
   This recurrence reproduces the sequence
   0,0,1,3,6,9,14,19,25,32,40,48,58,68,79,91,104,117,132,147,163,180,198,216,...
   which matches A242771 for all n ≥ 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_val = 0;   /* result a(n) */
    seq_t d = 0;       /* current difference d(k) */

    if (n <= 1) {
        return 0;
    }

    for (idx_t i = 1; i < n; ++i) {
        a_val += d;   /* add a(n) = a(n-1) + d(n-1) */

        /* update d for next step using the period-6 pattern */
        idx_t p = (i - 1) % 6;
        switch (p) {
            case 0: d += 1; break;   /* +1 */
            case 1: d += 1; break;   /* +1 */
            case 2: d += 1; break;   /* +1 */
            case 3: /* +0 */ break;   /* +0 */
            case 4: d += 2; break;   /* +2 */
            case 5: /* +0 */ break;   /* +0 */
        }
    }

    return a_val;
}
