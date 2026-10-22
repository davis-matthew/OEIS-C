/*
 * A358761  Numbers k for which bigomega(k) == 1 (mod 4).
 * https://oeis.org/A358761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A358761: Numbers k with bigomega(k) ≡ 1 (mod 4) */
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for offset 1, but safe */
    idx_t count = 0;
    idx_t k = 2;             /* start from smallest positive integer >1 */
    seq_t result = 0;
    while (count < n) {
        seq_t omega = bigomega(k);
        if ((omega % (seq_t)4) == (seq_t)1) {
            count++;
            if (count == n) {
                result = k;
                break;
            }
        }
        k++;
    }
    return result;
}
