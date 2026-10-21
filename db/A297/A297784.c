/*
 * A297784  Number of distinct runs in base-16 digits of n.
 * https://oeis.org/A297784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct runs in the base‑16 representation of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* extract the hexadecimal digits of n, least significant first */
    uint8_t digits[16];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 16);
        tmp /= 16;
    }
    if (len == 0) { /* n == 0, not needed for the problem but harmless */
        digits[len++] = 0;
    }

    /* table of seen (digit, run length) pairs */
    bool seen[16][17] = { {0} };  /* 16 digits, run length 1..16 inclusive */

    int i = len - 1;          /* start from most significant digit */
    int distinct = 0;

    while (i >= 0) {
        uint8_t d = digits[i];
        int runlen = 0;
        while (i >= 0 && digits[i] == d) {
            ++runlen;
            --i;
        }
        if (!seen[d][runlen]) {
            seen[d][runlen] = true;
            ++distinct;
        }
    }

    return (seq_t)distinct;
}
