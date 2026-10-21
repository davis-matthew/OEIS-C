/*
 * A298475  Minimal size of a palindromic partition of the binary
 *          representation of n.
 * https://oeis.org/A298475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pal_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute minimal palindromic partition size of the binary representation
 * of n (OEIS A298475). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_pal_bin.h"

/* a(n): minimal number of binary palindromes that partition the binary
 * representation of n, with n >= 1.  For n == 0, returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n = 0, return 0 */

    /* Determine bit length of n */
    idx_t len = 0;
    idx_t t   = n;
    while (t) {
        ++len;
        t >>= 1;
    }

    /* Allocate buffer for binary string (MSB first) */
    char *s = (char *)malloc((size_t)len);
    if (!s)
        return (seq_t)0;                 /* out of memory */

    t = n;
    for (idx_t i = 0; i < len; ++i) {
        s[len - 1 - i] = (t & 1) ? '1' : '0';
        t >>= 1;
    }

    /* Dynamic programming: dp[i] = minimal partitions for suffix s[i..len-1] */
    seq_t dp[65];                         /* up to 64 bits + sentinel */
    dp[len] = (seq_t)0;
    for (idx_t i = len; i-- > 0;) {
        /* Use a safe initial large value; len+1 is guaranteed > any candidate. */
        seq_t best = (seq_t)(len + 1);
        for (idx_t j = i + 1; j <= len; ++j) {
            if (is_pal_bin(s + i, j - i)) {
                seq_t cand = (seq_t)1 + dp[j];
                if (cand < best)
                    best = cand;
            }
        }
        dp[i] = best;
    }

    seq_t result = dp[0];
    free(s);
    return result;
}
