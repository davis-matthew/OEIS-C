/*
 * A289380  Number of 10-digit numbers whose sum of digits is n.
 * https://oeis.org/A289380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = number of 10‑digit numbers with sum of digits n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computed via DP over 10 digits, leading digit 1–9, others 0–9 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_SUM 90   /* maximum possible sum for a 10‑digit number */

/* static DP table, lazily initialized */
static seq_t dp[11][MAX_SUM + 1];
static bool initialized = false;

/* Compute the sequence value for index n (1‑based offset) */
seq_t a(idx_t n)
{
    /* initialize DP once */
    if (!initialized) {
        /* base case */
        for (idx_t s = 0; s <= MAX_SUM; ++s)
            dp[0][s] = 0;
        dp[0][0] = 1;

        /* fill DP table */
        for (idx_t pos = 1; pos <= 10; ++pos) {
            idx_t low  = (pos == 1 ? 1 : 0);
            idx_t high = 9;
            for (idx_t s = 0; s <= MAX_SUM; ++s) {
                seq_t total = 0;
                for (idx_t d = low; d <= high; ++d) {
                    if (s >= d)
                        total += dp[pos - 1][s - d];
                }
                dp[pos][s] = total;
            }
        }
        initialized = true;
    }

    /* return the precomputed value (zero if n is out of range) */
    if (n > 0 && n <= MAX_SUM)
        return dp[10][n];
    else
        return (seq_t)0;
}
