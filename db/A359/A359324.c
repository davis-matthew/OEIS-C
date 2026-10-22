/*
 * A359324  Number of divisors of 6*n-2 of form 6*k+5.
 * https://oeis.org/A359324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (6*n-2) of form 6*k+5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not in the OEIS offset but harmless */

    /* Compute 6*n-2 = 6*n - 2 = 6*n - 2 = 6*n-2, equivalently 2*(3*n-1) */
    uint64_t NN = (uint64_t)n;
    uint64_t num = 3 * NN - 1;     /* 3*n-1 */

    /* Remove the factor 2 to isolate the odd part */
    while ((num & 1ULL) == 0) num >>= 1;
    uint64_t odd = num;

    if (odd == 1) return 0;        /* only divisor is 1, not 5 mod 6 */

    /* DP over residues modulo 6: dp[r] = number of ways to get residue r */
    seq_t dp[6] = {0, 1, 0, 0, 0, 0};   /* empty product gives residue 1 */

    uint64_t temp = odd;
    /* trial‑division by odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* Compute contributions of powers of p modulo 6 */
            seq_t ways[6] = {0};
            int pmod = (int)(p % 6);           /* p is odd, so pmod ∈ {1,5} */
            int val = 1;                       /* p^0 == 1 */
            for (int a = 0; a <= exp; ++a) {
                ways[val] += 1;
                val = (val * pmod) % 6;
            }
            /* Convolve dp with ways */
            seq_t newdp[6] = {0};
            for (int r = 0; r < 6; ++r) {
                if (dp[r] == 0) continue;
                for (int v = 0; v < 6; ++v) {
                    if (ways[v] == 0) continue;
                    int nr = (r * v) % 6;
                    newdp[nr] += dp[r] * ways[v];
                }
            }
            for (int i = 0; i < 6; ++i) dp[i] = newdp[i];
        }
    }
    /* Remaining factor, if any, is a prime > sqrt(original) */
    if (temp > 1) {
        uint64_t p = temp;
        int exp = 1;
        seq_t ways[6] = {0};
        int pmod = (int)(p % 6);
        int val = 1;
        for (int a = 0; a <= exp; ++a) {
            ways[val] += 1;
            val = (val * pmod) % 6;
        }
        seq_t newdp[6] = {0};
        for (int r = 0; r < 6; ++r) {
            if (dp[r] == 0) continue;
            for (int v = 0; v < 6; ++v) {
                if (ways[v] == 0) continue;
                int nr = (r * v) % 6;
                newdp[nr] += dp[r] * ways[v];
            }
        }
        for (int i = 0; i < 6; ++i) dp[i] = newdp[i];
    }

    return dp[5];   /* residue 5 modulo 6 */
}
