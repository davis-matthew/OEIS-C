/*
 * A265061  Coordination sequence for (2,4,6) tiling of hyperbolic plane.
 * https://oeis.org/A265061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 67, u64 n <= 139,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   For n ≥ 9:
     a(n) = a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + 2*a(n-5)
            - a(n-6) + a(n-7) - a(n-8)
   Initial terms:
     a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=12, a(5)=17, a(6)=24,
     a(7)=33, a(8)=45
   The recurrence is valid for all n ≥ 9.                                                       */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[9] = {
        1, 3, 5, 8, 12, 17, 24, 33, 45
    };
    if (n < 9) return base[(size_t)n];

    seq_t buf[8];
    size_t i;
    for (i = 0; i < 8; ++i)
        buf[i] = base[i + 1];          /* buf[0] = a(1), ..., buf[7] = a(8) */

    for (idx_t k = 9; k <= n; ++k) {
        seq_t next = buf[7] - buf[6] + 2u * buf[5] - buf[4]
                   + 2u * buf[3] - buf[2] + buf[1] - buf[0];

        for (i = 0; i < 7; ++i)       /* shift left: drop buf[0] */
            buf[i] = buf[i + 1];
        buf[7] = next;
    }

    return buf[7];
}
