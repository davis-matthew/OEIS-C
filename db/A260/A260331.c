/*
 * A260331  Labelings of n diamond-shaped posets with 4 vertices per diamond
 *          where the labels follow the poset relations.
 * https://oeis.org/A260331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4 n)! / 12^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Terms of the product (4k)(4k-1)(4k-2)(4k-3) / 12 */
        seq_t t1 = (seq_t)(4 * k);          /* divisible by 4 */
        t1 /= 4;                            /* divide the 4 factor */

        seq_t t2 = (seq_t)(4 * k - 1);
        seq_t t3 = (seq_t)(4 * k - 2);
        seq_t t4 = (seq_t)(4 * k - 3);

        /* Find a term divisible by 3 and divide it */
        if (t2 % 3 == 0)
            t2 /= 3;
        else if (t3 % 3 == 0)
            t3 /= 3;
        else
            t4 /= 3;                       /* guaranteed to be divisible by 3 */

        /* Multiply the reduced terms */
        res *= t1;
        res *= t2;
        res *= t3;
        res *= t4;
    }

    return res;
}
