/*
 * A319808  a(n) = A319725(n) + A319656(n).
 * https://oeis.org/A319808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A319725(n) + A319656(n), where A319725 and A319656 sort
 *            the base-8 digits of n into decreasing and increasing order
 *            respectively
 * Range:     u8 n <= 67, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 84 terms through n = 4038 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A319808_desc(idx_t n)
{
    unsigned char cnt[8] = {0};
    idx_t t = n;
    seq_t res = 0;
    while (t > 0) {
        cnt[t & 7]++;
        t >>= 3;
    }
    for (int d = 7; d >= 0; --d) {
        unsigned c = cnt[d];
        while (c--) {
            res = res * 8 + d;
        }
    }
    return res;
}

static seq_t A319808_asc(idx_t n)
{
    unsigned char cnt[8] = {0};
    idx_t t = n;
    seq_t res = 0;
    while (t > 0) {
        cnt[t & 7]++;
        t >>= 3;
    }
    for (int d = 0; d < 8; ++d) {
        unsigned c = cnt[d];
        while (c--) {
            res = res * 8 + d;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    return A319808_desc(n) + A319808_asc(n);
}
