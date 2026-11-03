/*
 * A279484  Expansion of Product_{k>=1} (1-x^(k^3)).
 * https://oeis.org/A279484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{S⊆Cubes, sum(S)=n} (-1)^|S|, where Cubes = {1, 8, 27,
 *            64, …} = {k³ | k ≥ 1}
 * Range:     i8 n <= 40130, i16 n <= 100000, i32 n <= 100000, i64 n <= 100000,
 *            i128 n <= 100000, big any n
 * Validated: standard, pass, 101 terms through n = 8533 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279484: Expansion coefficients of the product
   ∏_{k≥1} (1 - x^(k³))

   Implementation: subset‑sum DP with sign.  Complexity
   O(n·∛n) time and O(n) memory, works for any n ≥ 0
   and for any seq_t type (signed integer or arbitrary‑precision class). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1 */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return (seq_t)0;          /* allocation failure → 0 */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each cube k³ ≤ n */
    for (idx_t k = 1;; ++k) {
        __int128 cube128 = (__int128)k * k * k;   /* prevent overflow */
        if (cube128 > n || cube128 <= 0) break;   /* overflow guard */
        idx_t cube = (idx_t)cube128;

        /* In‑place DP update (descending) */
        for (idx_t i = n; i >= cube; --i) {
            dp[i] = dp[i] - dp[i - cube];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
