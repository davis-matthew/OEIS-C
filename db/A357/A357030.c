/*
 * A357030  a(n) is the number of integers in 0..n having nonincreasing
 *          digits.
 * https://oeis.org/A357030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function counts numbers ≤ n whose decimal digits are nonincreasing. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (most significant first). */
    uint8_t digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* dp[tight][started][prev] */
    seq_t dp[2][2][10] = {0};
    seq_t ndp[2][2][10] = {0};

    dp[1][0][0] = (seq_t)1;

    for (size_t pos = 0; pos < len; ++pos) {
        for (int t = 0; t < 2; ++t)
            for (int s = 0; s < 2; ++s)
                for (int p = 0; p < 10; ++p)
                    ndp[t][s][p] = (seq_t)0;

        for (int tight = 0; tight < 2; ++tight) {
            for (int started = 0; started < 2; ++started) {
                for (int prev = 0; prev < 10; ++prev) {
                    seq_t ways = dp[tight][started][prev];
                    if (ways == (seq_t)0)
                        continue;
                    int limit = tight ? digits[pos] : 9;
                    for (int dig = 0; dig <= limit; ++dig) {
                        int ntight = tight && (dig == digits[pos]);

                        if (!started) {
                            if (dig == 0) {
                                ndp[ntight][0][0] += ways;
                            } else {
                                ndp[ntight][1][dig] += ways;
                            }
                        } else {
                            if (dig > prev)
                                continue;
                            ndp[ntight][1][dig] += ways;
                        }
                    }
                }
            }
        }

        for (int t = 0; t < 2; ++t)
            for (int s = 0; s < 2; ++s)
                for (int p = 0; p < 10; ++p)
                    dp[t][s][p] = ndp[t][s][p];
    }

    seq_t result = (seq_t)0;
    for (int tight = 0; tight < 2; ++tight)
        for (int started = 0; started < 2; ++started)
            for (int prev = 0; prev < 10; ++prev)
                result += dp[tight][started][prev];

    return result;
}
