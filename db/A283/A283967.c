/*
 * A283967  Positions of 0 in A283966; complement of A284015.
 * https://oeis.org/A283967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A283966, the fixed point of
 *            0->1, 1->10101 (built by iterating the morphism)
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9419 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    idx_t target = (idx_t)3 * n + (idx_t)10;
    idx_t len = 1;
    unsigned char *cur = (unsigned char *)malloc(1);
    if (!cur) return (seq_t)0;
    cur[0] = 1;

    while (len < target) {
        idx_t newlen = 0;
        for (idx_t i = 0; i < len; ++i)
            newlen += cur[i] ? 5 : 1;
        unsigned char *nx = (unsigned char *)malloc((size_t)newlen);
        if (!nx) { free(cur); return (seq_t)0; }
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (cur[i]) {
                nx[pos++] = 1; nx[pos++] = 0; nx[pos++] = 1;
                nx[pos++] = 0; nx[pos++] = 1;
            } else {
                nx[pos++] = 1;
            }
        }
        free(cur);
        cur = nx;
        len = newlen;
    }

    idx_t zeros = 0;
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < len; ++i) {
        if (!cur[i]) {
            ++zeros;
            if (zeros == n) {
                result = (seq_t)(i + 1);
                break;
            }
        }
    }
    free(cur);
    return result;
}
