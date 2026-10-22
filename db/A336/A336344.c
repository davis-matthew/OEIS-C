/*
 * A336344  Number of k's, 1 <= k <= n where gcd(n,k) = 1, such that the
 *          continued fraction for n^2/k^2 has twice as many elements as the
 *          continued fraction for n/k.
 * https://oeis.org/A336344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   cf_len, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counter for OEIS A336344
   a(n) = #{1 ≤ k ≤ n | gcd(n,k)=1 and len( n^2 / k^2 ) = 2 * len( n / k ) },
   where len(p/q) is the number of partial quotients in the simple continued
   fraction of the rational p/q. */

/* Dependencies: the gcd function from utils/gcd.h must be available. */
#include "../utils/gcd.h"
#include "../utils/cf_len.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2, safe for n ≤ 10000 */
    for (idx_t k = 1; k <= n; ++k) {
        /* skip non-coprime k */
        if (gcd((seq_t)n, (seq_t)k) != 1)
            continue;

        seq_t len = cf_len((seq_t)n, (seq_t)k);
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t len_sq = cf_len(n_sq, k_sq);
        if (len_sq == len * 2)
            count++;
    }
    return count;
}
