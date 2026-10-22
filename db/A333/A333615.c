/*
 * A333615  a(n) is the number of ways to express 2*n+1 as a sum of parts x
 *          such that x+2 is an odd prime.
 * https://oeis.org/A333615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A333615:
   a(n) = number of partitions of 2*n+1 into parts of the form p-2,
   where p is an odd prime. The implementation uses a simple
   coin‑change dynamic programming approach. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Target sum */
    uint64_t N = (uint64_t)n * 2 + 1;

    /* Ensure the DP array is large enough */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    if (dp_cap <= N) {
        dp = (seq_t *)realloc(dp, (N + 1) * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure */
        dp_cap = N + 1;
    }
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        dp[i] = (seq_t)0;

    /* Build sieve up to limit = N + 2 */
    const uint64_t limit = N + 2;
    static bool *sieve = NULL;
    static size_t sieve_cap = 0;
    if (sieve_cap <= limit) {
        sieve = (bool *)realloc(sieve, (limit + 1) * sizeof(bool));
        if (!sieve) return (seq_t)0;          /* allocation failure */
        sieve_cap = limit + 1;
        for (size_t i = 0; i <= limit; ++i)
            sieve[i] = true;
        sieve[0] = sieve[1] = false;
        uint64_t root = (uint64_t)sqrt((double)limit);
        for (uint64_t p = 2; p <= root; ++p) {
            if (sieve[p]) {
                for (uint64_t m = p * p; m <= limit; m += p)
                    sieve[m] = false;
            }
        }
    }

    /* DP update: for each odd prime p, part = p-2 */
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (!sieve[p]) continue;
        uint64_t s = p - 2;
        if (s > N) continue;
        for (uint64_t i = s; i <= N; ++i)
            dp[i] = dp[i] + dp[i - s];
    }

    return dp[N];
}
