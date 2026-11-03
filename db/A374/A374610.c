/*
 * A374610  Smallest result of n in "base-p" using digits 0,1,2 and
 *          interpreted as base-3.
 * https://oeis.org/A374610
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 61 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute a(n) by a bounded knapsack DP.
   For each prime p = prime[i], we can use 0,1,2 copies.
   The cost of using d copies is d * 3^i.  The DP finds the minimum total
   cost to reach each sum up to n.  The routine uses the helper
   build_primes_upto() to generate the primes <= n and mul_small()
   to multiply the weight 3^i. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* sequence is defined for n>=2 */

    /* ---------------------------------------------------- */
    /* 1. Generate primes up to n (inclusive)               */
    /* ---------------------------------------------------- */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)n, &primes, &prime_cnt);
    if (prime_cnt == 0) return 0;   /* should not happen for n>=2 */

    /* ---------------------------------------------------- */
    /* 2. Pre‑compute weights 3^i for each prime index i   */
    /* ---------------------------------------------------- */
    seq_t *weight = mem_new(seq_t, prime_cnt);
    if (!weight) { free(primes); return 0; }
    weight[0] = (seq_t)1;
    for (size_t i = 1; i < prime_cnt; ++i)
        weight[i] = mul_small(weight[i-1], 3u);

    /* ---------------------------------------------------- */
    /* 3. DP arrays: dp[s] = minimal cost to reach sum s   */
    /* ---------------------------------------------------- */
    size_t N = (size_t)n + 1;       /* sums 0 .. n          */
    seq_t *dp   = mem_new(seq_t, N);
    bool *reach = (bool*)calloc(N, sizeof(bool));
    if (!dp || !reach) { free(primes); mem_free(weight); mem_free(dp); free(reach); return 0; }

    reach[0] = true;
    dp[0]    = (seq_t)0;

    /* ---------------------------------------------------- */
    /* 4. Process each prime (bounded with multiplicity 2) */
    /* ---------------------------------------------------- */
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint64_t p = primes[k];
        seq_t w = weight[k];
        for (size_t s = n; s-- > 0; ) {          /* iterate backwards */
            if (!reach[s]) continue;
            for (int d = 1; d <= 2; ++d) {
                size_t t = s + p * (size_t)d;
                if (t > n) break;
                seq_t inc = mul_small(w, (unsigned)d);
                seq_t cand = dp[s] + inc;
                if (!reach[t] || cand < dp[t]) {
                    reach[t] = true;
                    dp[t] = cand;
                }
            }
        }
    }

    /* ---------------------------------------------------- */
    /* 5. Clean up and return result                        */
    /* ---------------------------------------------------- */
    seq_t result = reach[n] ? dp[n] : (seq_t)0;
    free(primes);
    mem_free(weight);
    mem_free(dp);
    free(reach);
    return result;
}
