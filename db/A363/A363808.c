/*
 * A363808  Number of divisors of n of the form 7*k + 6.
 * https://oeis.org/A363808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   count of divisors of n congruent to 6 modulo 7 (a(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  // safety, though offset starts at 1
    uint64_t temp = n;
    seq_t dp[7] = {0};
    dp[1 % 7] = 1;  // divisor 1 has residue 1

    /* factor n by trial division */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0) continue;

        uint32_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        /* frequency of residues of powers of p modulo 7 */
        seq_t freq[7] = {0};
        uint64_t pmod = p % 7;
        uint64_t val   = 1 % 7;
        for (uint32_t k = 0; k <= exp; ++k) {
            freq[val] += 1;
            val = (val * pmod) % 7;
        }

        /* combine with existing dp */
        seq_t newdp[7] = {0};
        for (int i = 0; i < 7; ++i) if (dp[i]) {
            for (int j = 0; j < 7; ++j) if (freq[j]) {
                int idx = (i * j) % 7;
                newdp[idx] += dp[i] * freq[j];
            }
        }
        for (int i = 0; i < 7; ++i) dp[i] = newdp[i];
    }

    if (temp > 1) {  /* remaining prime factor with exponent 1 */
        seq_t freq[7] = {0};
        uint64_t pmod = temp % 7;
        uint64_t val   = 1 % 7;
        freq[val] += 1;           /* exponent 0 */
        val = (val * pmod) % 7;
        freq[val] += 1;           /* exponent 1 */

        seq_t newdp[7] = {0};
        for (int i = 0; i < 7; ++i) if (dp[i]) {
            for (int j = 0; j < 7; ++j) if (freq[j]) {
                int idx = (i * j) % 7;
                newdp[idx] += dp[i] * freq[j];
            }
        }
        for (int i = 0; i < 7; ++i) dp[i] = newdp[i];
    }

    return dp[6];
}
