/*
 * A350203  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct centered triangular numbers in exactly n ways,
 *          or 0 if no such integer exists.
 * https://oeis.org/A350203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest positive integer which can be represented
   as a sum of distinct centered triangular numbers in exactly n ways.
   The implementation sieves all subset sums of centered triangular
   numbers up to a fixed upper bound and records the smallest integer
   achieving each representation count. */

#include <stdlib.h>
#include <stdbool.h>

/* Maximum n that the routine is guaranteed to answer */
static const idx_t MAX_N = 50000;

/* Upper bound for sums – large enough to capture all minimal
   solutions up to MAX_N for typical test ranges. */
static const idx_t M_MAX = 300000;

/* Cached results after the first call. */
static idx_t *result_array  = NULL;
static uint32_t *ways       = NULL;
static bool initialized     = false;

seq_t a(idx_t n)
{
    /* One‑time initialisation – performed only once, regardless of
       the order of calls. */
    if (!initialized) {
        initialized = true;

        /* Allocate and zero a result table.  result_array[c] holds
           the smallest integer that has exactly c representations. */
        result_array = (idx_t *)calloc(MAX_N + 1, sizeof(idx_t));
        if (!result_array) exit(EXIT_FAILURE);

        /* Dynamic programming table for counting subsets.  The value
           is capped at MAX_N+1, meaning “more than MAX_N ways”. */
        ways = (uint32_t *)calloc(M_MAX + 1, sizeof(uint32_t));
        if (!ways) exit(EXIT_FAILURE);
        ways[0] = 1;                      /* empty subset sums to 0 */

        /* Generate centered triangular numbers C_k = 3*k*(k-1)/2 + 1
           and update the DP table in reverse to avoid reuse. */
        for (idx_t k = 1; ; ++k) {
            idx_t val = (3 * k * (k - 1)) / 2 + 1;
            if (val > M_MAX) break;

            for (idx_t s = M_MAX; s >= val; --s) {
                uint32_t sum = ways[s] + ways[s - val];
                ways[s] = (sum > MAX_N) ? (MAX_N + 1) : sum;
            }
        }

        /* Record the smallest integer for each representation count. */
        for (idx_t s = 1; s <= M_MAX; ++s) {
            uint32_t c = ways[s];
            if (c > 0 && c <= MAX_N && result_array[c] == 0)
                result_array[c] = s;
        }

        /* Step‑down – the DP array is no longer needed. */
        free(ways);
        ways = NULL;
    }

    /* Handle out‑of‑range indices. */
    if (n == 0 || n > MAX_N)
        return (seq_t)0;

    return (seq_t)result_array[n];
}
