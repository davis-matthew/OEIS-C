/*
 * A284914  Positions of 1 in A284912; complement of A284913.
 * https://oeis.org/A284914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284912, the fixed point of the
 *            morphism 0 -> 01, 1 -> 1010. The code builds the word in place
 *            (w = sigma(w), starting from sigma(0) = 01) and scans it; since
 *            2n - a(n) is in {0,1}, a prefix of length 2n+2 is enough
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8591 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t L = 2 * n + 2;
    char *buf = (char *)malloc((size_t)(L + 8));
    if (!buf)
        return (seq_t)0;
    /* w = sigma(w): sigma(w[0]) = "01" fills positions 0..1, and
       sigma(w[j]) for j >= 1 is appended in order */
    buf[0] = 0;
    buf[1] = 1;
    idx_t len = 2, j = 1;
    while (len < L) {
        if (buf[j] == 0) {
            buf[len++] = 0;
            buf[len++] = 1;
        } else {
            buf[len++] = 1;
            buf[len++] = 0;
            buf[len++] = 1;
            buf[len++] = 0;
        }
        j++;
    }
    idx_t cnt = 0, res = 0;
    for (idx_t i = 0; i < len; i++) {
        if (buf[i]) {
            cnt++;
            if (cnt == n) {
                res = i + 1;
                break;
            }
        }
    }
    free(buf);
    return (seq_t)res;
}
