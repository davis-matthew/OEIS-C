/*
 * A386778  a(n) = n^2*sigma_8(n).
 * https://oeis.org/A386778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * sigma_8(n) (sigma_8(n) = sum_{d|n} d^8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7129,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 9888 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute i^8 using seq_t arithmetic */
static inline seq_t pow8_i(idx_t i)
{
    seq_t a = (seq_t)i;
    seq_t a2 = a * a;      /* a^2   */
    seq_t a4 = a2 * a2;    /* a^4   */
    seq_t a8 = a4 * a4;    /* a^8   */
    return a8;
}

/* Cached sigma_8 values */
static seq_t *sigma = NULL;   /* sigma[0] ... sigma[max] */
static idx_t sigma_max = 0;   /* highest index computed */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    /* Expand cache if needed */
    if (n > sigma_max) {
        /* New maximum we need */
        idx_t new_max = n;
        size_t new_size = (size_t)new_max + 1;
        seq_t *new_arr = mem_resize(seq_t, sigma, new_size);
        if (!new_arr) {
            /* Allocation failure – abort (program environment expected) */
            exit(1);
        }
        sigma = new_arr;

        /* Initialize new entries to 0 */
        for (idx_t i = sigma_max + 1; i <= new_max; ++i)
            sigma[i] = 0;

        /* Old maximum */
        idx_t old_max = sigma_max;

        /* Compute contributions for i = 1 .. new_max */
        for (idx_t i = 1; i <= new_max; ++i) {
            seq_t val = pow8_i(i);

            /* Determine first multiple j > old_max */
            idx_t start;
            if (i <= old_max) {
                start = ((old_max / i) + 1) * i;
            } else { /* i > old_max */
                start = i;
            }

            for (idx_t j = start; j <= new_max; j += i)
                sigma[j] += val;
        }

        sigma_max = new_max;
    }

    /* Now compute a(n) = n^2 * sigma_8(n) */
    seq_t sq = (seq_t)n * (seq_t)n;
    return sq * sigma[n];
}
