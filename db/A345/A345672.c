/*
 * A345672  a(n) is the least m such that the decimal expansion of n appears
 *          in the concatenation of m, ..., m+k for some k >= 0 and Sum_{i =
 *          m..m+k-1} A055642(i) < A055642(n) <= Sum_{i = m..m+k} A055642(i).
 * https://oeis.org/A345672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m such that the decimal expansion of n appears in
 *            m, m+1, m+2, ... concatenated, starting inside the digits of m
 *            (found by brute-force search over m = 0..n)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int A345672_digits(idx_t v, unsigned char *out)
{
    unsigned char t[24];
    int len = 0;
    if (v == 0) { out[0] = 0; return 1; }
    while (v > 0) { t[len++] = (unsigned char)(v % 10); v /= 10; }
    for (int i = 0; i < len; i++) out[i] = t[len - 1 - i];
    return len;
}

seq_t a(idx_t n)
{
    unsigned char nb[24];
    unsigned char buf[160];
    int L = A345672_digits(n, nb);

    for (idx_t m = 0; m <= n; m++) {
        unsigned char mb[24];
        int lm = A345672_digits(m, mb);
        int need = lm - 1 + L;
        int len = 0;
        idx_t cur = m;
        while (len < need) {
            len += A345672_digits(cur, buf + len);
            cur++;
        }
        for (int off = 0; off < lm; off++) {
            if (buf[off] != nb[0]) continue;
            int ok = 1;
            for (int j = 1; j < L; j++) {
                if (buf[off + j] != nb[j]) { ok = 0; break; }
            }
            if (ok) return (seq_t)m;
        }
    }
    return (seq_t)n;
}
