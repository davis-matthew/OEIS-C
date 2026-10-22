/*
 * A350133  Last denominator in each run of odd terms in the greedy
 *          rearrangement of the alternating harmonic series that converges to
 *          2.
 * https://oeis.org/A350133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function implements the greedy “run‑reversal’’
 *            rearrangement of the alternating harmonic series that converges
 *            to 2. Each run consists of adding successive odd reciprocals
 *            until the partial sum exceeds 2, recording the last odd
 *            denominator used. After that the smallest unused even
 *            reciprocal...
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Ensure an adequate limit for the number of precomputed terms.  The tests
   use n up to about 10 000; a slightly larger buffer gives headroom. */
#define MAX_TERMS 20000

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a sentinel. */
    if (n < 1) {
        return 0;
    }

    /* Static state preserved across calls. */
    static idx_t cur_n = 0;                /* number of terms already computed */

    /* Cache of computed a(k).  The array size is fixed – sufficient for
       all practical calls. */
    static seq_t terms[MAX_TERMS];

    /* Current position of the next odd and even denominators to be used. */
    static uint64_t next_odd = 1;
    static uint64_t next_even = 2;

    /* Running sum of the rearranged series.  `long double` provides ample
       precision for the required range of n. */
    static long double sum = 0.0L;

    /* If the caller requests a term that we have already computed, return
       it immediately. */
    if (n <= cur_n) {
        return terms[n - 1];
    }

    /* Compute terms until we have at least n. */
    for (; cur_n < n; ++cur_n) {
        seq_t last_odd = (seq_t)next_odd;

        /* Add odd reciprocals until the sum exceeds 2. */
        while (true) {
            /* Add 1 / next_odd. */
            sum += 1.0L / (long double)next_odd;

            /* If the partial sum has crossed 2, record this odd and break. */
            if (sum > 2.0L) {
                last_odd = (seq_t)next_odd;
                next_odd += 2;
                break;
            }
            next_odd += 2;
        }

        /* Store the resulting denominator of this run. */
        terms[cur_n] = last_odd;

        /* Subtract the next unused even reciprocal to bring the sum below 2,
           and advance the even counter. */
        sum -= 1.0L / (long double)next_even;
        next_even += 2;
    }

    /* Return the requested term. */
    return terms[n - 1];
}
