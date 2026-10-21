/*
 * A293613  a(n) = (1/4)*(7*n + 17)*(5*n + 6)*Pochhammer(n, 6) / 6!.
 * https://oeis.org/A293613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(7*n + 17)*(5*n + 6)*Pochhammer(n, 6) / 6!
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 441, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = ((7*n+17)*(5*n+6)*n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)) / 2880 */

#include "../utils/gcd.h"
#include <stddef.h>

/* Helper to cancel a factor from an array of numerators */
static inline void cancel_factor(seq_t *nums, size_t len, seq_t fac)
{
    /* Repeatedly cancel all prime factors of fac */
    while (fac > 1) {
        for (size_t i = 0; i < len; ++i) {
            seq_t g = gcd(nums[i], fac);
            if (g > 1) {
                nums[i] /= g;
                fac    /= g;
                if (fac == 1)
                    break;
            }
        }
        /* If we get here and fac > 1, it means fac shares a divisor
           that is not found in the current numerators due to earlier
           cancellations. For the given sequence this cannot happen, so
           we just continue. */
    }
}

/* Computes a(n) for the sequence A293613 */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t nums[8];

    /* numerator factors */
    nums[0] = sn;                         /* n       */
    nums[1] = sn + 1;                     /* n+1     */
    nums[2] = sn + 2;                     /* n+2     */
    nums[3] = sn + 3;                     /* n+3     */
    nums[4] = sn + 4;                     /* n+4     */
    nums[5] = sn + 5;                     /* n+5     */
    nums[6] = 7 * sn + 17;                /* 7n+17   */
    nums[7] = 5 * sn + 6;                 /* 5n+6    */

    /* Cancel the factors of 2880 = 2^6 * 3^2 * 5 */
    cancel_factor(nums, 8, (seq_t)64);     /* cancel 2^6 */
    cancel_factor(nums, 8, (seq_t)9);      /* cancel 3^2 */
    cancel_factor(nums, 8, (seq_t)5);      /* cancel 5   */

    /* Multiply the remaining factors */
    seq_t result = 1;
    for (size_t i = 0; i < 8; ++i)
        result *= nums[i];

    return result;
}
