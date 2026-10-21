/*
 * A241617  Number of length n+2 0..11 arrays with no consecutive three
 *          elements summing to more than 11.
 * https://oeis.org/A241617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 arrays of digits 0..11 such that no
 *            three consecutive digits sum to more than 11. Offset: 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 12 possible values for a digit (0..11). */
static bool          dp_inited    = false;
static seq_t          dp_prev[12][12];
static seq_t          dp_cur [12][12];
static idx_t          dp_len       = 2;       /* current array length   */
static seq_t        *a_cache      = NULL;    /* a_cache[n] stores a(n) */
static idx_t          cached_n_max = 0;       /* largest n cached       */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 (outside the offset); return 0 safely. */
    if (n == 0) return 0;

    /* Initialise the DP for length 2: every pair of digits is allowed. */
    if (!dp_inited) {
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = 0; j < 12; ++j)
                dp_prev[i][j] = (seq_t)1;
        dp_inited = true;
    }

    /* Target length is n + 2 (arrays of length n+2). */
    const idx_t target_len = n + 2;
    while (dp_len < target_len) {
        /* Zero out dp_cur. */
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = 0; j < 12; ++j)
                dp_cur[i][j] = (seq_t)0;

        /* Extend every valid pair with a new digit. */
        for (idx_t prev = 0; prev < 12; ++prev) {
            for (idx_t cur = 0; cur < 12; ++cur) {
                seq_t val = dp_prev[prev][cur];
                if (val == (seq_t)0) continue;
                idx_t sum = prev + cur;
                if (sum > 11) continue;
                for (idx_t nxt = 0; nxt < 12; ++nxt) {
                    if (sum + nxt <= 11) {
                        dp_cur[cur][nxt] += val;
                    }
                }
            }
        }

        /* Prepare for next iteration. */
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = 0; j < 12; ++j)
                dp_prev[i][j] = dp_cur[i][j];
        ++dp_len;

        /* If we have reached a length corresponding to an n, cache its value. */
        if (dp_len >= 3) {
            idx_t curr_n = dp_len - 2;          /* because L = n+2 */
            seq_t total = (seq_t)0;
            for (idx_t i = 0; i < 12; ++i)
                for (idx_t j = 0; j < 12; ++j)
                    total += dp_prev[i][j];

            if (curr_n > cached_n_max) {
                size_t new_size = (size_t)curr_n + 1; /* index 0 unused */
                seq_t *tmp = (seq_t *)realloc(a_cache,
                                               new_size * sizeof(seq_t));
                if (!tmp) abort();                  /* OOM */
                a_cache = tmp;
                cached_n_max = curr_n;
            }
            a_cache[curr_n] = total;
        }
    }

    /* dp_len now equals target_len; a_cache[n] holds the result. */
    return a_cache[n];
}
