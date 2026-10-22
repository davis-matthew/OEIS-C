/*
 * A326456  Sum of the smallest parts of the partitions of n into 8 primes.
 * https://oeis.org/A326456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 179, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326456: Sum of the smallest parts of the partitions of n into 8 primes.
   Computation method:
   For each possible smallest prime p, count partitions of n‑p into 7 primes,
   each ≥ p, using a dynamic programming table over primes processed in
   decreasing order.  The contribution of p to the sum is p multiplied by
   that count.  The algorithm is O(#primes·n·7) and uses a 2‑dimensional
   table DP[t][s] = number of ways to write s as a sum of t primes,
   all ≥ the current smallest prime in the loop.  */
 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
 
seq_t a(idx_t n) {
    /* For n < 8 no partition into 8 primes exists */
    if (n < 8) return (seq_t)0;
 
    size_t N = (size_t)n;            /* safe as n ≤ 10000 in tests */
    size_t Np1 = N + 1;
 
    /* ---------- 1. Sieve of Eratosthenes up to N ----------------------- */
    unsigned char *sieve = (unsigned char*)calloc(Np1, sizeof(unsigned char));
    if (!sieve) return (seq_t)0;
    for (size_t i = 2; i <= N; ++i) sieve[i] = 1;
    for (size_t i = 2; (size_t)i * i <= N; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= N; j += i) sieve[j] = 0;
 
    /* collect primes into a dynamic array */
    size_t estimate = N / 10 + 10;                      /* rough over‑estimate */
    uint64_t *primes = (uint64_t*)malloc(estimate * sizeof(uint64_t));
    if (!primes) { free(sieve); return (seq_t)0; }
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i)
        if (sieve[i]) {
            if (prime_cnt >= estimate) {               /* grow if needed */
                estimate <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes,
                                                    estimate * sizeof(uint64_t));
                if (!tmp) { free(primes); free(sieve); return (seq_t)0; }
                primes = tmp;
            }
            primes[prime_cnt++] = (uint64_t)i;
        }
    free(sieve);   /* sieve no longer needed */
 
    /* ---------- 2. DP table: dp[t][s]  (t = 0..7, s = 0..N) ----------- */
    size_t dp_size = 8 * Np1;
    seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(primes); return (seq_t)0; }
    /* helper for indexing */
    #define IDX(t,s) ((t) * Np1 + (s))
 
    dp[IDX(0,0)] = (seq_t)1;           /* one way to sum to 0 with 0 primes */
 
    seq_t ans = (seq_t)0;
 
    /* ---------- 3. Process primes in descending order ----------------- */
    for (size_t pi = prime_cnt; pi > 0; --pi) {
        uint64_t p = primes[pi - 1];
 
        /* Update DP to allow using prime p (unbounded coin use). */
        for (size_t t = 1; t <= 7; ++t) {
            for (size_t s = p; s <= N; ++s) {
                dp[IDX(t,s)] += dp[IDX(t-1, s - (size_t)p)];
            }
        }
 
        /* After processing p, dp[7][N-p] counts partitions of N-p
           into 7 primes each ≥ p.  Add the contribution of the smallest
           part p. */
        if (N >= p) {
            seq_t ways = dp[IDX(7, N - (size_t)p)];
            ans += (seq_t)p * ways;
        }
    }
 
    #undef IDX
    free(dp);
    free(primes);
    return ans;
}
