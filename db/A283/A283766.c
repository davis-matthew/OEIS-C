/*
 * A283766  Numbers k such that L(k) is odd, where L = A000201 = lower Wythoff
 *          sequence.
 * https://oeis.org/A283766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 6266, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that L(k) = floor(k*φ) is odd, where φ = (1+√5)/2.
   L(k) can be computed without floating point as  
       L(k) = ⌊ (k + ⌊√(5k²)⌋ ) / 2 ⌋.
   The parity of L(k) is then used to select the desired k. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in practice */

    idx_t  found = 0;
    seq_t  k = 0;                   /* candidate k */

    while (found < n) {
        k++;                      /* next k */

        /* m = ⌊√(5k²)⌋  (integer square root)                     */
        seq_t m = floor_sqrt((seq_t)5 * k * k);

        /* L(k) = ⌊(k + m)/2⌋                                         */
        seq_t Lk = (k + m) / 2;

        if ((Lk & 1) != 0) {       /* L(k) odd */
            found++;
            if (found == n) return k;
        }
    }

    return 0;                     /* unreachable for n>=1 */
}
