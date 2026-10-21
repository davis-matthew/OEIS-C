/*
 * A295744  Numbers that have exactly four representations of a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A295744: numbers that have exactly four unordered representations as a sum of
   seven nonnegative squares. The function precomputes all such numbers up to
   5408 by a dynamic‑programming over the set of square values, stores them
   in a static array and returns the nth element. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N      5408   /* largest number that may appear in the sequence */
#define MAX_TERMS  500    /* generous upper bound on the final list size */

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static bool built = false;
    static seq_t terms[MAX_TERMS];
    static size_t term_count = 0;

    if (!built) {
        /* dp[sum][k] = number of unordered ways to form 'sum' using exactly
           'k' squares from the processed set of square values.  k ranges
           from 0 to 7, sum from 0 to MAX_N.  unsigned long long is
           sufficient because the largest coefficient is
           C(73+7-1,7) = C(79,7) < 6.1·10⁷. */
        static uint64_t dp[MAX_N + 1][8];

        /* initialise */
        dp[0][0] = 1;

        /* iterate over square values v = i^2 (including 0) */
        for (int i = 0; i * i <= MAX_N; ++i) {
            int sq = i * i;
            /* update counts for all sums that can include this square */
            for (int sum = sq; sum <= MAX_N; ++sum) {
                for (int k = 1; k <= 7; ++k) {
                    dp[sum][k] += dp[sum - sq][k - 1];
                }
            }
        }

        /* collect all numbers having exactly four representations */
        for (int sum = 0; sum <= MAX_N; ++sum) {
            if (dp[sum][7] == 4) {
                if (term_count < MAX_TERMS) {
                    terms[term_count++] = (seq_t)sum;
                }
            }
        }

        built = true;
    }

    if (n == 0 || (size_t)n > term_count)
        return (seq_t)0;          /* undefined beyond the finite list */

    return terms[(size_t)n - 1];
}
