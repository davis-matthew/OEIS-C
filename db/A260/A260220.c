/*
 * A260220  Number of symmetry-allowed, linearly-independent terms at n-th
 *          order in the expansion of T1 x t1 rovibrational perturbation
 *          matrix H(Jx,Jy,Jz).
 * https://oeis.org/A260220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = (n+1)(n+2)/2 a(4n+1) = (n+1)^2 a(4n+3) = (n+1)(n+2)
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t res;
    /* even indices */
    if ((n & 1) == 0) {
        seq_t k = (seq_t)(n >> 1);            /* k = n/2 */
        res = (k + 1) * (k + 2) / 2;
    } else { /* odd indices */
        if ((n % 4) == 1) {                   /* n = 4k+1 */
            seq_t k = (seq_t)((n - 1) >> 2);  /* k = (n-1)/4 */
            res = (k + 1) * (k + 1);
        } else {                              /* n = 4k+3 */
            seq_t k = (seq_t)((n - 3) >> 2);  /* k = (n-3)/4 */
            res = (k + 1) * (k + 2);
        }
    }
    return res;
}
