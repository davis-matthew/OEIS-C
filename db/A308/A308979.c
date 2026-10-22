/*
 * A308979  Sum of the third largest parts in the partitions of n into 7
 *          primes.
 * https://oeis.org/A308979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the third largest parts in the partitions of n into 7 primes.
   The DP keeps the number of partitions for each (k, s) and,
   when k==5, accumulates the 5th (i.e. third largest) part.
   Complexity: O(n × π(n) × 7) – about 90 M operations for n=10 000. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ---------- sieve primes up to n ----------
       We use a byte per entry. 0 = prime, 1 = composite.
    */
    size_t N = (size_t)n + 1;
    bool *is_composite = (bool*)calloc(N, sizeof(bool));
    if (!is_composite) return (seq_t)0;
    for (size_t i = 2; i * i <= n; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_composite[j] = true;

    /* ---------- DP tables ----------
       dp_count[k][s] – number of partitions of sum s into k primes.
       dp_sum  [k][s] – sum of the 5th smallest part among those partitions.
    */
    seq_t *dp_count[8];
    seq_t *dp_sum[8];
    for (int k = 0; k <= 7; ++k) {
        dp_count[k] = (seq_t*)calloc(N, sizeof(seq_t));
        dp_sum[k]   = (seq_t*)calloc(N, sizeof(seq_t));
        if (!dp_count[k] || !dp_sum[k]) {             /* allocation failure */
            free(is_composite);
            for (int t = 0; t <= 7; ++t) {
                if (dp_count[t]) free(dp_count[t]);
                if (dp_sum[t])   free(dp_sum[t]);
            }
            return (seq_t)0;
        }
    }
    dp_count[0][0] = (seq_t)1;   /* one way to sum 0 with 0 primes */

    /* ---------- main DP ----------
       Iterate over primes in ascending order.  For each prime p,
       we allow it to be used any number of times (by looping s ascending).
    */
    for (size_t p = 2; p <= n; ++p) {
        if (is_composite[p]) continue;
        for (size_t s = p; s <= (size_t)n; ++s) {
            for (int k = 1; k <= 7; ++k) {
                seq_t prev_cnt = dp_count[k-1][s - p];
                if (!prev_cnt) continue;

                dp_count[k][s] += prev_cnt;

                if (k == 5) {
                    /* the 5th (third largest) part is the new prime p */
                    dp_sum[5][s] += prev_cnt * (seq_t)p;
                } else if (k > 5) {
                    /* the 5th part remains unchanged from the (k‑1)-partitions */
                    dp_sum[k][s] += dp_sum[k-1][s - p];
                }
                /* k < 5: no contribution to dp_sum */
            }
        }
    }

    seq_t result = dp_sum[7][n];

    /* ---------- cleanup ----------
       Free allocated memory.  The function returns a value that
       is owned by the caller (it may be stored for later use). */
    free(is_composite);
    for (int k = 0; k <= 7; ++k) {
        free(dp_count[k]);
        free(dp_sum[k]);
    }

    return result;
}
