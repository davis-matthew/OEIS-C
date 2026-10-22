/*
 * A351983  Number of integer compositions of n with exactly one part above
 *          the diagonal.
 * https://oeis.org/A351983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351983
    Numbers of integer compositions of n with exactly one part greater
    than its position index.
    The values are computed by dynamic programming over the length
    of the composition.  For each partial composition we keep track
    of whether an oversized part has already been used and update
    the possible next parts accordingly.
*/

#include <stdlib.h>
#include <stddef.h>

/*  seq_t and idx_t are provided by the build environment.  */

seq_t a(idx_t n)
{
    /*  For n = 0 or 1 there is no valid composition.  */
    if (n <= 1)
        return (seq_t)0;

    /*  Use size_t for array indices – n fits into size_t for the
        problem sizes considered.  */
    size_t N = (size_t)n;
    size_t sz = N + 2;           /* arrays of size N+2 (indices 0..N+1)  */

    /*  Allocate working arrays.  */
    seq_t *dp0  = (seq_t *)calloc(N + 1, sizeof(seq_t));   /* no oversized part yet */
    seq_t *dp1  = (seq_t *)calloc(N + 1, sizeof(seq_t));   /* exactly one oversized part */
    seq_t *n0   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *n1   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *inc0 = (seq_t *)calloc(sz,     sizeof(seq_t)); /* starts of intervals for dp0 */
    seq_t *dec0 = (seq_t *)calloc(sz,     sizeof(seq_t)); /* ends+1 of intervals for dp0 */
    seq_t *inc1 = (seq_t *)calloc(sz,     sizeof(seq_t)); /* starts of intervals for dp1 */
    seq_t *dec1 = (seq_t *)calloc(sz,     sizeof(seq_t)); /* ends+1 of intervals for dp1 */

    /*  Start with the empty composition: one way, sum 0, 0 parts.  */
    dp0[0] = (seq_t)1;

    for (size_t pos = 0; pos < N; ++pos) {
        /*  Clear all boundary arrays for this step.  */
        for (size_t i = 0; i < sz; ++i) {
            inc0[i] = dec0[i] = inc1[i] = dec1[i] = (seq_t)0;
        }

        /*  Consider all compositions that currently have 'pos' parts
            and sum <= N.  For each possible next part x we update
            the appropriate interval using the start/end arrays.  */
        for (size_t sum = pos; sum <= N; ++sum) {
            seq_t val0 = dp0[sum];
            if (val0) {
                /*  Parts <= pos+1 keep the sequence in state 0.  */
                size_t l0 = sum + 1;
                size_t r0 = sum + pos + 1;
                if (r0 > N) r0 = N;
                if (l0 <= r0) {
                    inc0[l0] += val0;
                    if (r0 + 1 < sz) dec0[r0 + 1] += val0;
                }

                /*  Parts > pos+1 introduce the unique oversized part.  */
                size_t l1 = sum + pos + 2;
                if (l1 <= N) {
                    inc1[l1] += val0;
                    /*  All contributions end after N (i.e., at N+1).  */
                    if (N + 1 < sz) dec1[N + 1] += val0;
                }
            }

            seq_t val1 = dp1[sum];
            if (val1) {
                /*  In state 1 we can only add parts <= pos+1.  */
                size_t l1 = sum + 1;
                size_t r1 = sum + pos + 1;
                if (r1 > N) r1 = N;
                if (l1 <= r1) {
                    inc1[l1] += val1;
                    if (r1 + 1 < sz) dec1[r1 + 1] += val1;
                }
            }
        }

        /*  Build the new dp arrays by prefix summing the intervals.
            The difference between starts and ends gives the exact count
            for each sum.  */
        seq_t acc_inc0 = 0, acc_dec0 = 0, acc_inc1 = 0, acc_dec1 = 0;
        for (size_t i = 0; i <= N; ++i) {
            acc_inc0 += inc0[i];
            acc_dec0 += dec0[i];
            n0[i] = acc_inc0 - acc_dec0;      /* acc_inc0 >= acc_dec0 by construction */

            acc_inc1 += inc1[i];
            acc_dec1 += dec1[i];
            n1[i] = acc_inc1 - acc_dec1;
        }

        /*  Keep compositions that have already finished at sum = N.  */
        n1[N] += dp1[N];
        n0[N] += dp0[N];

        /*  Prepare for the next step: swap dp and new arrays.  */
        seq_t *tmp = dp0; dp0 = n0; n0 = tmp;
        tmp = dp1; dp1 = n1; n1 = tmp;
    }

    seq_t result = dp1[N];

    /*  Free all allocated memory.  */
    free(dp0); free(dp1); free(n0); free(n1);
    free(inc0); free(dec0); free(inc1); free(dec1);

    return result;
}
