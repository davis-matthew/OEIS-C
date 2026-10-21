/*
 * A245039  Number of partitions of n where the minimal multiplicity of any
 *          part is 8.
 * https://oeis.org/A245039
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which the minimal *
 *            multiplicity of any part equals 8. * * Computed as (# partitions
 *            of n where every part appears at * least 8 times) minus (#
 *            partitions where every part appears at * least 9 times). The
 *            calculation is performed with dynamic * programming over...
 * Range:     u8 n <= 91, u16 n <= 191, u32 n <= 490, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/compute_dp.h"

/* ------------------------------------------------------------------ */
/* DP for partitions in which every part appears at least 'm' times.  */

/* ------------------------------------------------------------------ */
/* Main interface required by the tests.                               */
/* ------------------------------------------------------------------ */
static idx_t maxComputed = 0;
static seq_t *dp8 = NULL;
static seq_t *dp9 = NULL;

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* If we haven't computed up to 'n' yet, recompute both DP tables
     * from scratch up to the new maximum index. */
    if (n > maxComputed) {
        idx_t newN = n;

        seq_t *newdp8 = (seq_t *)calloc(newN + 1, sizeof(seq_t));
        seq_t *newdp9 = (seq_t *)calloc(newN + 1, sizeof(seq_t));

        compute_dp(newN, 8, newdp8);
        compute_dp(newN, 9, newdp9);

        free(dp8);
        free(dp9);
        dp8 = newdp8;
        dp9 = newdp9;
        maxComputed = newN;
    }

    /* Result is the difference between the two DP tables. */
    return dp8[n] - dp9[n];
}
