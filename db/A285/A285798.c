/*
 * A285798  Number of partitions of n into parts with an even number of
 *          distinct prime divisors.
 * https://oeis.org/A285798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts having an even
 *            number of distinct prime divisors
 * Range:     u8 n <= 44, u16 n <= 109, u32 n <= 297, u64 n <= 895,
 *            u128 n <= 2937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:  generate all admissible parts by a small sieve,
   then perform a classic integer partition dynamic program.
   The result is memoised so that repeated calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of partition numbers up to whatever maximum
       index has been requested so far. */
    static seq_t *dp = NULL;     /* dp[i] = #partitions of i */
    static idx_t max_n = 0;      /* maximum n computed so far     */

    /* If the cache is empty or the new request exceeds it,
       rebuild everything up to n. */
    if (dp == NULL || n > max_n)
    {
        /* Use size_t for array sizes (will be a 64‑bit type on
           the target platforms). */
        size_t m  = (size_t)n;
        size_t sz = m + (size_t)1;      /* include 0 .. n */

        /* Allocate auxiliary arrays.  Use explicit casts so the
           code compiles in both C and C++ modes. */
        unsigned char *cnt      = (unsigned char*)calloc(sz, sizeof(unsigned char));
        bool          *allowed  = (bool*)malloc(sz * sizeof(bool));
        if (!cnt || !allowed) return 0; /* allocation failed */

        /* Sieve to count distinct prime factors for every number. */
        for (size_t i = 2; i <= m; ++i)
        {
            if (cnt[i] == 0)          /* i is prime          */
            {
                for (size_t j = i; j <= m; j += i)
                    ++cnt[j];
            }
        }

        /* Determine admissible parts: even number of distinct primes
           (by convention 1 has 0 distinct primes, which is even). */
        allowed[0] = false;
        for (size_t i = 1; i <= m; ++i)
            allowed[i] = ((cnt[i] & 1u) == 0u);

        /* Partition DP: dp[s] accumulates the number of ways to
           obtain sum s using admissible parts.  Start with dp[0]=1. */
        seq_t *new_dp = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!new_dp) return 0; /* allocation failed */
        new_dp[0] = 1;

        for (size_t p = 1; p <= m; ++p)
        {
            if (!allowed[p]) continue;
            for (size_t s = p; s <= m; ++s)
                new_dp[s] += new_dp[s - p];
        }

        /* Clean up temporary arrays. */
        free(cnt);
        free(allowed);

        /* Replace old cache if it existed. */
        if (dp) free(dp);
        dp    = new_dp;
        max_n = n;
    }

    return dp[n];
}
