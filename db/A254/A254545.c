/*
 * A254545  Number of length 7 1..(n+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑7 arrays with entries 1..(n+2) such that
 *            * no partial sum is prime
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Maximum entry value and maximum possible partial sum. */
    size_t max_val = (size_t)n + 2;
    size_t max_sum = 7 * max_val;

    /* Prime sieve up to max_sum. */
    bool *isPrime = (bool *)calloc(max_sum + 1, sizeof(bool));
    if (!isPrime) return (seq_t)0;
    for (size_t i = 2; i <= max_sum; ++i) isPrime[i] = true;
    for (size_t i = 2; i * i <= max_sum; ++i) {
        if (!isPrime[i]) continue;
        for (size_t j = i * i; j <= max_sum; j += i) isPrime[j] = false;
    }

    /* DP: dp_prev[s] = number of prefix ways reaching sum s after k elements. */
    size_t prev_max = 0;
    seq_t *dp_prev = (seq_t *)calloc(prev_max + 1, sizeof(seq_t));
    if (!dp_prev) { free(isPrime); return (seq_t)0; }
    dp_prev[0] = (seq_t)1;

    for (size_t step = 1; step <= 7; ++step) {
        size_t curr_max = step * max_val;
        seq_t *dp_curr = (seq_t *)calloc(curr_max + 1, sizeof(seq_t));
        if (!dp_curr) { free(dp_prev); free(isPrime); return (seq_t)0; }

        /* Prefix sums of dp_prev for efficient range queries. */
        seq_t *prefix = (seq_t *)calloc(prev_max + 1, sizeof(seq_t));
        if (!prefix) { free(dp_curr); free(dp_prev); free(isPrime); return (seq_t)0; }
        if (prev_max >= 0) {
            prefix[0] = dp_prev[0];
            for (size_t i = 1; i <= prev_max; ++i) prefix[i] = prefix[i - 1] + dp_prev[i];
        }

        for (size_t t = 1; t <= curr_max; ++t) {
            size_t l = (t > max_val) ? t - max_val : 0;
            size_t r = t - 1;
            if (l > prev_max) continue;
            if (r > prev_max) r = prev_max;
            seq_t sum = prefix[r];
            if (l > 0) sum -= prefix[l - 1];
            if (!isPrime[t]) dp_curr[t] = sum;
        }

        free(prefix);
        free(dp_prev);
        dp_prev = dp_curr;
        prev_max = curr_max;
    }

    /* Total count after 7 steps. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i <= prev_max; ++i) total += dp_prev[i];

    free(dp_prev);
    free(isPrime);
    return total;
}
