/*
 * A251231  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A251231: Number of (n+1)×(3+1) 0..3 arrays with
   every 2×2 subblock summing to 6 and no 2×2 subblock having exactly
   two non‑zero entries.

   Empirical recurrence (valid for n ≥ 12):
     a(n) = 13*a(n-1) – 63*a(n-2) + 122*a(n-3) + 13*a(n-4)
            – 371*a(n-5) + 317*a(n-6) + 308*a(n-7)
            – 370*a(n-8) – 144*a(n-9) + 128*a(n-10) + 48*a(n-11)

   Initial values:
     a(1)=322,  a(2)=694,  a(3)=1592,  a(4)=3886,  a(5)=9914,
     a(6)=26224, a(7)=71322, a(8)=198318, a(9)=561212,
     a(10)=1611002, a(11)=4679574.
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If requested index is below the offset, return the
       corresponding initial term. */
    static const seq_t init[11] = {
        322, 694, 1592, 3886, 9914,
        26224, 71322, 198318, 561212, 1611002, 4679574
    };

    if (n <= 11)
        return init[n - 1];

    /* Buffer containing the last 11 terms of the recurrence.
       The values are initialized with the known first 11 terms.
       Each iteration computes a(n) from the preceding 11 terms
       and shifts the buffer left by one position. */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t next =
            13 * buf[10] - 63 * buf[9] + 122 * buf[8] + 13 * buf[7]
            - 371 * buf[6] + 317 * buf[5] + 308 * buf[4]
            - 370 * buf[3] - 144 * buf[2] + 128 * buf[1] + 48 * buf[0];

        /* Shift left: buf[0] becomes buf[1], buf[1] becomes buf[2], ... */
        for (idx_t k = 0; k < 10; ++k)
            buf[k] = buf[k + 1];
        buf[10] = next;
    }

    return buf[10];
}
