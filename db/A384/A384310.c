/*
 * A384310  Numbers k such that A383844(k) and A383844(k+1) are nonzero.
 * https://oeis.org/A384310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 0 such that A383844(m) > 0 and A383844(m+1) >
 *            0, where A383844(m) = #{k : sum_{prime p<=k} (k mod p) = m};
 *            computed by direct enumeration of k and a doubling search bound
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    const int CAP = 20000;
    static unsigned char comp[20001];
    static int primes[3000];
    static int pc = 0;
    static bool init = false;
    if (!init) {
        for (int i = 2; i <= CAP; ++i) {
            if (!comp[i]) {
                primes[pc++] = i;
                for (long long j = (long long)i * i; j <= CAP; j += i) comp[j] = 1;
            }
        }
        init = true;
    }

    uint64_t M = 1024;
    for (;;) {
        unsigned char *cnt = (unsigned char *)calloc((size_t)M + 2, 1);
        int streak = 0;
        int np = 0;
        for (int k = 0; k <= CAP; ++k) {
            while (np < pc && primes[np] <= k) ++np;
            uint64_t s = 0;
            for (int i = 0; i < np; ++i) s += (uint64_t)(k % primes[i]);
            if (s <= M) {
                cnt[s] = 1;
                streak = 0;
            } else {
                ++streak;
                if (k >= 2000 && streak > 20) break;
            }
        }
        uint64_t found = 0;
        for (uint64_t m = 0; m < M; ++m) {
            if (cnt[m] && cnt[m + 1]) {
                ++found;
                if (found == (uint64_t)n) {
                    free(cnt);
                    return (seq_t)m;
                }
            }
        }
        free(cnt);
        M *= 4;
    }
}
