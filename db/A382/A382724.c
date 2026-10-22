/*
 * A382724  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 6.
 * https://oeis.org/A382724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #k with 0 ≤ k ≤ n for which C(n,k) is not divisible by 6
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp2_of, exp3_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. C(n,k) is not simultaneously even and divisible by 3). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/exp2_of.h"
#include "../utils/exp3_of.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* tracked valuations of 2 and 3 in the current binomial coefficient */
    int v2 = 0;
    int v3 = 0;

    /* Iterate over all k from 0 to n */
    for (idx_t k = 0; ; ++k)
    {
        /* Count the coefficient if it is not divisible by 6 */
        if (v2 == 0 || v3 == 0)
            result++;

        if (k == n)
            break;

        /* Transition to C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        idx_t num = n - k;
        idx_t den = k + 1;

        v2 += (int)exp2_of(num) - (int)exp2_of(den);
        v3 += (int)exp3_of(num) - (int)exp3_of(den);
    }

    return result;
}
