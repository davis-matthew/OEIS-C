/*
 * A300190  Number of solutions to 1 +- 2 +- 3 +- ... +- n == 0 (mod n).
 * https://oeis.org/A300190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to 1 ± 2 ± 3 ± … ± n ≡ 0 (mod n)
 *            (with the sign of 1 fixed +). The number is counted by taking
 *            the subset P of indices {2,…,n} that receive the minus sign and
 *            requiring 2*sum(P) ≡ n(n+1)/2 (mod n). For odd n this reduces to
 *            sum(P) ≡ 0 (mod n); for n ≡ 2 (mod 4) there ar...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 is beyond the offset, return 0 */
    if (n == 0) return 0;

    /* Determine modulus and target residue */
    idx_t mod, target;
    if (n & 1) {                       /* n odd */
        mod = n;
        target = 0;
    } else if ((n & 3) == 2) {         /* n ≡ 2 (mod 4) */
        return 0;
    } else {                           /* n ≡ 0 (mod 4) */
        mod = n / 2;
        target = n / 4;
    }

    /* Allocate DP arrays */
    seq_t *dp     = (seq_t *)calloc(mod, sizeof(seq_t));
    seq_t *next   = (seq_t *)calloc(mod, sizeof(seq_t));
    if (!dp || !next) return 0;           /* out‑of‑memory guard */

    /* Base: empty subset */
    dp[0] = 1;

    /* Iterate over elements 2..n (indices that may be negative) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Current value modulo mod */
        idx_t val = i % mod;

        /* Copy current dp to next (empty subset not chosen) */
        for (idx_t k = 0; k < mod; ++k)
            next[k] = dp[k];

        /* Add contributions for choosing element i */
        for (idx_t j = 0; j < mod; ++j) {
            idx_t k = j + val;
            if (k >= mod) k -= mod;
            next[k] += dp[j];
        }

        /* Swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[target];

    free(dp);
    free(next);
    return result;
}
