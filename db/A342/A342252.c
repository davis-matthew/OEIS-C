/*
 * A342252  Number of ways to write n as an ordered sum of nine powers of 2.
 * https://oeis.org/A342252
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k≥0} x^{2^k})^9
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache and working arrays */
static seq_t *answers = NULL;
static seq_t *dp_prev = NULL, *dp_curr = NULL;
static idx_t max_n = 8;          /* initially no values >=9 are cached */

/* Recompute all values up to target */
static void recompute(idx_t target)
{
    if (target <= max_n) return;

    idx_t new_max = target;

    /* Preserve old arrays for freeing later */
    seq_t *old_answers = answers;
    seq_t *old_dp_prev = dp_prev;
    seq_t *old_dp_curr = dp_curr;

    /* Allocate new arrays */
    answers   = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
    dp_prev   = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
    dp_curr   = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));

    /* Initialise dp_prev */
    for (idx_t i = 0; i <= new_max; ++i) dp_prev[i] = (seq_t)0;
    dp_prev[0] = (seq_t)1;

    /* Precompute powers of two up to new_max */
    idx_t powers[64];
    idx_t pcount = 0;
    for (idx_t p = 1; p <= new_max; p <<= 1) {
        powers[pcount++] = p;
    }

    /* Dynamic programming for 9 parts */
    for (int k = 1; k <= 9; ++k) {
        /* Zero dp_curr */
        for (idx_t i = 0; i <= new_max; ++i) dp_curr[i] = (seq_t)0;

        /* Accumulate contributions from each power of two */
        for (idx_t pi = 0; pi < pcount; ++pi) {
            idx_t power = powers[pi];
            for (idx_t s = power; s <= new_max; ++s) {
                dp_curr[s] += dp_prev[s - power];
            }
        }

        /* Swap dp_prev and dp_curr for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Copy final results into answers */
    for (idx_t i = 0; i <= new_max; ++i) answers[i] = dp_prev[i];

    /* Free old arrays */
    free(old_answers);
    free(old_dp_prev);
    free(old_dp_curr);

    max_n = new_max;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n > max_n) recompute(n);
    return answers[n];
}
