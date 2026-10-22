/*
 * A327399  Number of factorizations of n that are constant or whose distinct
 *          factors are pairwise coprime.
 * https://oeis.org/A327399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A327399: Number of factorizations of n that are constant or whose distinct factors are pairwise coprime.
    Implementation: brute‑force enumeration of all unordered factorizations of n.
    The algorithm recurses over divisors d ≥ min_factor, producing each factorization exactly once.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */

/* Recursive helper that enumerates unordered factorizations of n with factors ≥ min_factor.
   fac[] holds the current partial factor list, sz is its length.
   cnt is updated with each admissible factorization found. */
static void dfs(uint64_t n, uint64_t min_factor, uint64_t fac[], size_t sz, seq_t *cnt)
{
    /* Try all divisors d of n that are ≥ min_factor and ≤ sqrt(n). */
    for (uint64_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            fac[sz] = d;
            dfs(n / d, d, fac, sz + 1, cnt);
        }
    }

    /* Final factor n itself (ending the current factorization). */
    if (n >= min_factor) {
        fac[sz] = n;

        bool all_equal = true;
        for (size_t i = 1; i <= sz; ++i) {
            if (fac[i] != fac[0]) { all_equal = false; break; }
        }

        if (all_equal) {
            (*cnt)++;
        } else {
            bool ok = true;
            for (size_t i = 0; i <= sz && ok; ++i) {
                for (size_t j = i + 1; j <= sz; ++j) {
                    if (fac[i] == fac[j]) continue;            /* duplicate factors are allowed */
                    if (gcd((seq_t)fac[i], (seq_t)fac[j]) != 1) {  /* distinct factors must be coprime */
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) (*cnt)++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;   /* by definition a(1) = 1 */

    uint64_t N = (uint64_t)n;   /* n fits in uint64_t for our target range */
    seq_t result = 0;
    uint64_t stack[64];   /* depth dust 14 for n <= 10000, 64 is ample */

    dfs(N, 2, stack, 0, &result);
    return result;
}
