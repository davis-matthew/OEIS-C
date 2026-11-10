/*
 * A288519  a(n) = least k such that A080670(k) begins with n.
 * https://oeis.org/A288519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least k such that A080670(k) begins with n; found by a
 *            linear search over k, comparing the decimal digit string of
 *            A080670(k) with the digits of n
 * Range:     u8 n <= 19, u16 n <= 661, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8812 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static size_t A288519_put(char *buf, size_t pos, uint64_t v)
{
    char tmp[24];
    size_t t = 0;
    do { tmp[t++] = (char)('0' + (int)(v % 10)); v /= 10; } while (v);
    while (t) buf[pos++] = tmp[--t];
    return pos;
}

seq_t a(idx_t n)
{
    char pre[24];
    size_t plen = A288519_put(pre, 0, (uint64_t)n);
    uint64_t k;
    for (k = 1; ; ++k) {
        char buf[512];
        size_t len = 0;
        if (k == 1) {
            buf[len++] = '1';
        } else {
            uint64_t pr[64], ex[64];
            size_t cnt = 0, i;
            factor_u64(k, pr, ex, &cnt);
            for (i = 0; i < cnt; ++i) {
                len = A288519_put(buf, len, pr[i]);
                if (ex[i] > 1) len = A288519_put(buf, len, ex[i]);
            }
        }
        if (len >= plen) {
            size_t j;
            bool ok = true;
            for (j = 0; j < plen; ++j) {
                if (buf[j] != pre[j]) { ok = false; break; }
            }
            if (ok) return (seq_t)k;
        }
    }
}
