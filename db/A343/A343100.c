/*
 * A343100  Number of partitions of n into 3 parts, at least 2 of which are
 *          coprime.
 * https://oeis.org/A343100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts such that at least
 *            two of the parts are coprime
 * Range:     u8 n <= 58, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the standard gcd implementation from the utils library. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* No partition into three positive parts when n < 3. */
    if (n < 3)
        return 0;

    seq_t count = 0;

    /* i <= j <= k, i + j + k = n */
    for (idx_t i = 1; i <= n / 3; ++i) {
        /* j ranges from i up to (n - i)/2 to ensure j <= k. */
        idx_t jmax = (n - i) / 2;
        for (idx_t j = i; j <= jmax; ++j) {
            idx_t k = n - i - j;   /* guaranteed j <= k */

            seq_t ai = (seq_t)i;
            seq_t aj = (seq_t)j;
            seq_t ak = (seq_t)k;

            /* Count the partition if any pair is coprime. */
            if ((gcd(ai, aj) == 1) ||
                (gcd(ai, ak) == 1) ||
                (gcd(aj, ak) == 1)) {
                ++count;
            }
        }
    }
    return count;
}
