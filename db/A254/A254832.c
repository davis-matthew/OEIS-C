/*
 * A254832  Number of length 5 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count arrays of length 5 over 1..(n+1) with every prefix sum
 *            divisible by 2, 3 or 5. Computed by dynamic programming over
 *            remainders modulo 30 (the lcm of 2, 3, 5)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 114, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Residues modulo 30 that are divisible by 2, 3, or 5. */
static const unsigned char good_mod[30] = {
    1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1,
    1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0
};

seq_t a(idx_t n)
{
    /* Numbers available for each position: 1 .. n+1. */
    idx_t N = n + 1;
    unsigned int count[30];

    /* Count of numbers with each residue modulo 30. */
    idx_t div = N / 30;
    unsigned int rem = N % 30;
    for (int r = 0; r < 30; ++r) {
        if (r == 0) {
            count[0] = (unsigned int)div;  /* multiples of 30 */
        } else {
            count[r] = (unsigned int)(div + ((rem >= (unsigned int)r) ? 1u : 0u));
        }
    }

    /* dp[pos][mod] = ways to reach remainder `mod` after `pos` elements. */
    seq_t dp[6][30];
    /* Initialize all entries to zero. */
    for (int i = 0; i < 6; ++i) {
        for (int m = 0; m < 30; ++m) {
            dp[i][m] = (seq_t)0;
        }
    }
    dp[0][0] = (seq_t)1;   /* empty prefix has sum 0 */

    for (int pos = 0; pos < 5; ++pos) {
        for (int mod = 0; mod < 30; ++mod) {
            seq_t cur = dp[pos][mod];
            if (cur == (seq_t)0) continue;
            for (int r = 0; r < 30; ++r) {
                unsigned int cnt = count[r];
                if (cnt == 0u) continue;
                int new_mod = (mod + r) % 30;
                if (!good_mod[new_mod]) continue;
                dp[pos + 1][new_mod] += cur * (seq_t)cnt;
            }
        }
    }

    /* Sum over all allowed final remainders. */
    seq_t result = (seq_t)0;
    for (int mod = 0; mod < 30; ++mod) {
        result += dp[5][mod];
    }
    return result;
}
