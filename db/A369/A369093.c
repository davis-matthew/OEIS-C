/*
 * A369093  Numbers k >= 1 such that sigma(k) divides the sum of the
 *          triangular numbers T(k) and T(k+1), where sigma(k) = A000203(k) is
 *          the sum of the divisors of k.
 * https://oeis.org/A369093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k ≥ 1 such that sigma(k) divides
 *            (k+1)^2. * Here sigma(k) is the sum of the positive divisors of
 *            k. This amounts to * testing whether (k+1)^2 % sigma(k) == 0.
 *            The function scans positive * integers in order, counting those
 *            that satisfy the condition, until the * required i...
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"

/* External function required by the judge */
seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of valid terms found so far   */
    idx_t k     = 1;            /* candidate integer to test            */
    seq_t  result = 0;          /* the value to return (k in seq_t)     */

    while (count < n) {
        seq_t sigma = sigma_of(k);            /* σ(k) */
        seq_t kp1   = (seq_t)(k + 1);         /* k+1    */
        seq_t kp1sq = kp1 * kp1;              /* (k+1)^2 */

        if (kp1sq % sigma == 0) {             /* condition satisfied */
            count++;
            if (count == n) {
                result = (seq_t)k;
                break;
            }
        }
        k++;
    }
    return result;
}
