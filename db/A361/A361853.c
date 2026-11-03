/*
 * A361853  Number of integer partitions of n such that (length) * (maximum) =
 *          2n.
 * https://oeis.org/A361853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts partitions of n such that (length)*(maximum)=2n
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed standard headers */
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------
   Count partitions of integer 'sum' into exactly 'k' parts, each part
   not exceeding 'maxv', with at least one part equal to 'maxv'.
   ------------------------------------------------------------------ */
static seq_t count_exact_k_max_eq_m(idx_t sum, idx_t k, idx_t maxv)
{
    /* Quick rejects.  If k > sum or maxv > sum the condition can never be met. */
    if (k == 0) return (sum == 0) ? 1 : 0;
    if (sum < k || maxv == 0 || maxv > sum) return 0;

    /* dp[j][s] – number of partitions of 's' into exactly 'j' parts,
       each part ≤ current processed value ('maxv' or less). */
    size_t rows = (size_t)k + 1;          /* j = 0 … k   */
    size_t cols = (size_t)sum + 1;        /* s = 0 … sum */
    size_t cells = rows * cols;

    /* Allocate contiguous block; use calloc to zero initialise */
    seq_t *dp = mem_new(seq_t, cells);
    if (!dp) return 0;            /* allocation failed – return 0 */

    /* Base: one way to split 0 with 0 parts */
    dp[0] = 1;

    /* Process part sizes 1 .. maxv-1 first (contributions that do NOT use a part = maxv) */
    if (maxv > 1) {
        for (idx_t part = 1; part <= maxv - 1; ++part) {
            for (idx_t s = part; s <= sum; ++s) {
                size_t base = s;
                for (idx_t j = 1; j <= k; ++j) {
                    dp[j * cols + base] += dp[(j - 1) * cols + (base - part)];
                }
            }
        }
    }

    /* Record value before adding the maximum part size 'maxv' */
    seq_t before = dp[k * cols + sum];

    /* Process part size = maxv (allows unlimited copies of this part) */
    for (idx_t s = maxv; s <= sum; ++s) {
        for (idx_t j = 1; j <= k; ++j) {
            dp[j * cols + s] += dp[(j - 1) * cols + (s - maxv)];
        }
    }

    seq_t after = dp[k * cols + sum];

    mem_free(dp);
    return after - before;   /* partitions with max part exactly 'maxv' */
}

/* ------------------------------------------------------------------
   Main entry: a(n)
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t result = 0;
    idx_t two_n = 2 * n;

    /* enumerate all factor pairs (k, m) of 2n */
    for (idx_t d = 1; d * d <= two_n; ++d) {
        if (two_n % d != 0) continue;
        idx_t k1 = d;          /* length */
        idx_t m1 = two_n / d;  /* maximum part */

        /* first pair (k1, m1) */
        if (k1 <= n && m1 <= n) {
            result += count_exact_k_max_eq_m(n, k1, m1);
        }

        /* second pair (k2, m2), unless d*d == two_n */
        if (d * d == two_n) continue;
        idx_t k2 = two_n / d;
        idx_t m2 = d;
        if (k2 <= n && m2 <= n) {
            result += count_exact_k_max_eq_m(n, k2, m2);
        }
    }

    return result;
}
