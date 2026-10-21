/*
 * A256820  Number of length n+5 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{8} p[j] * C(n - j + 4, 5) where p = {64, -256,
 *            448, -384, 124, 16, -7, -8, 4} and C(k,5) =
 *            k·(k-1)·(k-2)·(k-3)·(k-4)/120 for k≥5 (0 otherwise)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const int32_t p[9] = {64, -256, 448, -384, 124, 16, -7, -8, 4};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 the behavior is irrelevant. */
    __int128 sum = 0;

    for (int j = 0; j < 9; ++j) {
        /* The term contributes only when n ≥ j+1 (i.e. k = n-j+4 ≥ 5). */
        if (n < (idx_t)(j + 1))
            continue;

        idx_t k = n - j + 4;          /* k = n - j + 4 */
        /* Compute binomial coefficient C(k,5) as product/120. */
        __int128 bin = (__int128)k * (k - 1) * (k - 2) * (k - 3) * (k - 4);
        bin /= 120;                    /* exact division, k≥5 guarantees integer result */

        sum += (__int128)p[j] * bin;
    }

    return (seq_t)sum;
}
