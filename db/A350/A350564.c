/*
 * A350564  Areas of integer-sided triangles with hypotenuse A084645(n).
 * https://oeis.org/A350564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = area of the unique integer right triangle with hypotenuse
 *            h = A084645(n); h = p*m with p prime == 1 (mod 4) the only such
 *            prime factor (exponent 1), p = s^2+t^2, area =
 *            m^2*s*t*(s^2-t^2). Found by search
 * Range:     u8 n <= 10, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *hyp_list = NULL;
static size_t hyp_cnt = 0, hyp_cap = 0;
static uint64_t hyp_next = 5;

/* returns prime p if h has exactly one prime factor == 1 mod 4 with exponent 1, else 0 */
static uint64_t uniq_p(uint64_t h) {
    uint64_t m = h;
    while ((m & 1) == 0) m >>= 1;
    uint64_t p = 0;
    int cnt = 0;
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; e++; }
            if (d % 4 == 1) {
                if (e > 1) return 0;
                cnt++;
                p = d;
                if (cnt > 1) return 0;
            }
        }
    }
    if (m > 1 && m % 4 == 1) {
        cnt++;
        p = m;
    }
    return cnt == 1 ? p : 0;
}

seq_t a(idx_t n) {
    while (hyp_cnt < (size_t)n) {
        uint64_t h = hyp_next++;
        if (uniq_p(h)) {
            if (hyp_cnt >= hyp_cap) {
                hyp_cap = hyp_cap ? hyp_cap * 2 : 256;
                hyp_list = (uint64_t *)realloc(hyp_list, hyp_cap * sizeof(uint64_t));
            }
            hyp_list[hyp_cnt++] = h;
        }
    }
    uint64_t h = hyp_list[(size_t)(n - 1)];
    uint64_t p = uniq_p(h);
    uint64_t m = h / p;
    uint64_t s = 0, t = 0;
    for (uint64_t tt = 1; 2 * tt * tt < p; tt++) {
        uint64_t s2 = p - tt * tt;
        uint64_t r = 0;
        while ((r + 1) * (r + 1) <= s2) r++;
        if (r * r == s2) { s = r; t = tt; break; }
    }
    seq_t mm = (seq_t)m;
    seq_t area = mm * mm * (seq_t)s * (seq_t)t * (seq_t)(s * s - t * t);
    return area;
}
