/*
 * A295803  Numbers that have exactly six representations as a sum of seven
 *          positive squares.
 * https://oeis.org/A295803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have exactly six representations as a sum of seven positive squares.
   The function computes all such numbers up to a fixed bound (20000) via a
   dynamic‑programming coin‑change algorithm and returns the n‑th term. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *list = NULL;
    static size_t list_len = 0;

    if (!list) {
        /* Upper bound sufficient to capture all numbers with exactly six
           representations. Empirically, the bound can be set well below
           20000; 20000 is a safe choice. */
        const idx_t MAX = 20000;          /* maximum n to check          */
        const idx_t K   = 7;              /* number of squares to use    */
        const size_t stride = (size_t)(MAX + 1);

        /* Allocate DP table for counts, stored as uint8_t to avoid large
           arbitrary‑precision objects.  Values are saturated at 7
           (meaning >6). */
        uint8_t *dp = (uint8_t *)calloc((K + 1) * stride, sizeof(uint8_t));
        if (!dp) return 0; /* unlikely but guard against allocation failure */

        dp[0 * stride + 0] = 1; /* one way: 0 with 0 squares */

        /* Coin–change DP: add each square value once, allowing unlimited
           repetitions, but counting unordered multisets. */
        for (idx_t sq = 1; ; ++sq) {
            idx_t val = sq * sq;
            if (val > MAX) break;

            for (idx_t j = val; j <= MAX; ++j) {
                idx_t jv = j - val;
                for (idx_t k = 1; k <= K; ++k) {
                    uint8_t prev = dp[(k - 1) * stride + jv];
                    if (!prev) continue;
                    uint8_t cur  = dp[k * stride + j];
                    uint8_t sum = (uint8_t)(prev + cur);
                    if (sum > 6) sum = 7; /* saturation */
                    dp[k * stride + j] = sum;
                }
            }
        }

        /* Collect numbers that have exactly six representations. */
        size_t cnt = 0;
        for (idx_t j = 1; j <= MAX; ++j) {
            if (dp[K * stride + j] == 6) ++cnt;
        }

        list = (seq_t *)malloc(sizeof(seq_t) * cnt);
        if (!list) {
            free(dp);
            return 0;
        }

        size_t pos = 0;
        for (idx_t j = 1; j <= MAX; ++j) {
            if (dp[K * stride + j] == 6) {
                list[pos++] = (seq_t)j;
            }
        }
        list_len = cnt;
        free(dp);
    }

    /* Return the nth term (1‑based).  Return 0 for n out of bounds. */
    if (n == 0 || n > list_len) return 0;
    return list[n - 1];
}
