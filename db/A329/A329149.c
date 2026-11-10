/*
 * A329149  Integers that do not appear in A329147, in increasing order.
 * https://oeis.org/A329149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive k not in the image of A329147; k is in the
 *            image iff its decimal string parses as a concatenation of tokens
 *            0,2,3,5,7,11,13,17,19,23 (search by increasing k)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A329149_in_image(seq_t k)
{
    int s[48];
    int len = 0;
    seq_t t = k;
    while (t > 0) {
        s[len++] = (int)(t % 10);
        t /= 10;
    }
    /* reverse to most-significant-first */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        int x = s[i]; s[i] = s[j]; s[j] = x;
    }
    bool dp[50];
    for (int i = 0; i <= len; ++i) dp[i] = false;
    dp[0] = true;
    for (int i = 0; i < len; ++i) {
        if (!dp[i]) continue;
        int d = s[i];
        if (d == 0 || d == 2 || d == 3 || d == 5 || d == 7)
            dp[i + 1] = true;
        if (i + 1 < len) {
            int e = s[i + 1];
            if (d == 1 && (e == 1 || e == 3 || e == 7 || e == 9))
                dp[i + 2] = true;
            if (d == 2 && e == 3)
                dp[i + 2] = true;
        }
    }
    return dp[len];
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t k = 0;
    while (cnt < n) {
        ++k;
        if (!A329149_in_image(k))
            ++cnt;
    }
    return k;
}
