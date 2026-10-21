/*
 * A280263  G.f.: Product_{k>=1} (1+x^(k^3)) / (1-x^(k^3)).
 * https://oeis.org/A280263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 498, u32 n <= 3126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280263
   Product formula:   a(n) = [x^n] ∏_{k≥1} (1 + x^(k³)) / (1 - x^(k³))
   Equivalent combinatorial description: partitions of n into
   multiples of cubes with weight 2 for each distinct cube size used. */

#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    /* dp[i] will hold the coefficient for x^i after considering some cubes */
    seq_t *dp = alloc_seq(n + 1);
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t k = 1; ; ++k) {
        idx_t cube = k * k * k;
        if (cube > n) break;          /* no further cubes contribute */

        /* in‑place update: for each residue class modulo cube */
        for (idx_t r = 0; r < cube; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t idx = r; idx <= n; idx += cube) {
                seq_t old = dp[idx];           /* value before update */
                dp[idx] = old + (seq_t)2 * sum;/* dp'[idx] = old + 2*sum */
                sum += old;                     /* sum for next element */
            }
        }
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
