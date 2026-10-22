/*
 * A373404  Sum of the n-th maximal antirun of composite numbers differing by
 *          more than one.
 * https://oeis.org/A373404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the sum of the n‑th maximal antirun of composite
 * numbers (A002808) where consecutive terms differ by more than one.
 * The antiruns are maximal intervals of composites with gaps >1;
 * each run is stored as soon as it finishes. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *sums = NULL;            /* array of computed a(k) values   */
static size_t sum_cap = 0, sum_len = 0;
static uint64_t next_num = 4;         /* next integer to test           */
static bool run_active = false;       /* are we inside a run?           */
static seq_t run_sum = 0;             /* current run sum                */
static uint64_t prev_comp = 0;        /* last composite processed       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0            */

    /* generate more run sums until we have at least n of them */
    while (sum_len < n) {
        /* scan integers, skip primes, process composites */
        for (;;) {
            uint64_t m = next_num++;
            seq_t val = (seq_t)m;

            /* skip primes; only composites are relevant */
            if (is_prime_small(val))
                continue;

            if (!run_active) {
                /* start a new run */
                run_active = true;
                run_sum   = val;
            } else {
                uint64_t diff = m - prev_comp;
                if (diff > 1) {
                    /* continue current run */
                    run_sum += val;
                } else {
                    /* current run ends at prev_comp */
                    if (sum_len == sum_cap) {
                        sum_cap = sum_cap ? sum_cap * 2 : 16;
                        sums = (seq_t *)realloc(sums, sum_cap * sizeof(seq_t));
                    }
                    sums[sum_len++] = run_sum;

                    /* start new run with the current composite */
                    run_sum = val;
                }
            }

            prev_comp = m;

            if (sum_len >= n) /* we already have the requested term */
                return sums[n - 1];
        }
    }

    /* should never reach here, but return a placeholder */
    return 0;
}
