/*
 * A341205  Number of ways to write n as an ordered sum of 8 nonzero decimal
 *          palindromes.
 * https://oeis.org/A341205
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of ways to write n as an ordered sum of 8 non‑zero
 *            decimal palindromes (n ≥ 8)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 593, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_palindromes_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming:
   dp[k]  – number of ways to obtain the sum k using the processed
   number of parts.  For each part we convolve the current dp array
   with the set of palindromes ≤ n.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/generate_palindromes_u64.h"

/* ---------- Helper to create/index arrays of seq_t ---------- */
#ifdef __cplusplus
#define ALLOC_SEQ_ARRAY(len)      (new seq_t[(len)]())
#define FREE_SEQ_ARRAY(p)         delete[] (p)
#else
#define ALLOC_SEQ_ARRAY(len)      ( (seq_t*)calloc((len), sizeof(seq_t)) )
#define FREE_SEQ_ARRAY(p)         free (p)
#endif

/* ---------- Main OEIS function ---------- */
seq_t a(idx_t n)
{
    if (n < 8) return 0;                 /* offset of the sequence */

    size_t N = (size_t)n;                /* loop indices use size_t        */

    /* Allocate DP arrays. */
    seq_t *dp   = ALLOC_SEQ_ARRAY(N + 1);
    if (!dp) return 0;                    /* allocation failure             */
    dp[0] = 1;                            /* base case – one way to make 0   */

    /* Generate all non‑zero palindromes up to N. */
    uint64_t *pal_array = NULL;
    size_t pal_cnt = 0;
    generate_palindromes_u64(N, &pal_array, &pal_cnt);
    if (!pal_array) { FREE_SEQ_ARRAY(dp); return 0; }

    /* Perform DP for 8 parts. */
    for (int part = 0; part < 8; ++part) {
        seq_t *newdp = ALLOC_SEQ_ARRAY(N + 1);
        if (!newdp) {
            FREE_SEQ_ARRAY(dp);
            free(pal_array);
            return 0;
        }

        for (size_t pi = 0; pi < pal_cnt; ++pi) {
            size_t p = pal_array[pi];
            for (size_t sum = p; sum <= N; ++sum) {
                newdp[sum] += dp[sum - p];
            }
        }

        FREE_SEQ_ARRAY(dp);
        dp = newdp;
    }

    seq_t result = dp[N];

    /* Clean up. */
    FREE_SEQ_ARRAY(dp);
    free(pal_array);

    return result;
}
