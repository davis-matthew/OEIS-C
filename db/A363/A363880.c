/*
 * A363880  Number of divisors of 7*n-6 of form 7*k+3.
 * https://oeis.org/A363880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of 7*n-6 that are ≡ 3 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t primes_table[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263
};
static const size_t primes_cnt = sizeof(primes_table) / sizeof(primes_table[0]);

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Offset is 1; behaviour for n<1 irrelevant   */
    uint64_t N = 7ULL * n - 6ULL;        /* 7*n-6 fits easily into 64 bits             */

    /* Factor N; primes up to sqrt(N) are enough */
    uint64_t factors[10];
    int exponents[10];
    size_t sz = 0;
    uint64_t m = N;

    for (size_t i = 0; i < primes_cnt && primes_table[i]*primes_table[i] <= m; ++i) {
        if (m % primes_table[i] == 0) {
            int e = 0;
            while (m % primes_table[i] == 0) {
                m /= primes_table[i];
                ++e;
            }
            factors[sz] = primes_table[i];
            exponents[sz] = e;
            ++sz;
        }
    }
    if (m > 1) {
        factors[sz] = m;
        exponents[sz] = 1;
        ++sz;
    }

    /* DP over residues modulo 7 */
    seq_t dp[7] = {0};
    dp[1] = 1;                            /* empty product */
    seq_t newdp[7];

    for (size_t i = 0; i < sz; ++i) {
        seq_t freq[7] = {0};
        uint64_t power = 1;                 /* current p^k mod 7 */
        for (int k = 0; k <= exponents[i]; ++k) {
            freq[(int)(power % 7)] += 1;
            power = (power * factors[i]) % 7;
        }
        /* convolution modulo 7 */
        for (int r = 0; r < 7; ++r) newdp[r] = 0;
        for (int r1 = 0; r1 < 7; ++r1) if (dp[r1]) {
            for (int r2 = 0; r2 < 7; ++r2) if (freq[r2]) {
                int res = (r1 * r2) % 7;
                newdp[res] += dp[r1] * freq[r2];
            }
        }
        for (int r = 0; r < 7; ++r) dp[r] = newdp[r];
    }

    return dp[3];                          /* number of divisors ≡ 3 (mod 7) */
}
