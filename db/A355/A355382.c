/*
 * A355382  Number of divisors d of n such that bigomega(d) = omega(n).
 * https://oeis.org/A355382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n with bigomega(d) = omega(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard against n=0 */

    uint64_t n64 = (uint64_t)n;
    size_t  cnt = 0;
    uint64_t exps[64];
    uint64_t primes[64];                 /* not used, but needed by factor_u64 */
    factor_u64(n64, primes, exps, &cnt);

    int k = (int)cnt;                    /* omega(n) = number of distinct primes */

    seq_t ways[65] = {0};                /* up to k distinct primes <= 64 */
    ways[0] = 1;

    for (int i = 0; i < k; ++i) {
        uint64_t e = exps[i];
        seq_t next[65] = {0};

        for (int s = 0; s <= k; ++s) {
            seq_t val = ways[s];
            if (!val) continue;

            int maxf = (int)e;
            if (maxf > k - s) maxf = k - s;

            for (int f = 0; f <= maxf; ++f)
                next[s + f] += val;
        }

        for (int s = 0; s <= k; ++s)
            ways[s] = next[s];
    }

    return ways[k];
}
