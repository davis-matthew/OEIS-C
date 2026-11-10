/*
 * A365425  The least number with the prime signature of the odd part of
 *          A163511(n): a(n) = A046523(A000265(A163511(n))).
 * https://oeis.org/A365425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A000265(A163511(n))); computed from the binary
 *            gaps of n: the odd part of A163511(n) has exponents gap_i (+1
 *            for the top prime p(r)) on p(2..r), and these exponents sorted
 *            descending go onto 2,3,5,
 * Range:     u8 n <= 256, u16 n <= 4680, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 113 terms through n = 9679 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A365425_pow(uint64_t base, uint64_t e)
{
    seq_t r = 1;
    seq_t b = (seq_t)base;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t ex[70];
    int r = 0;
    uint64_t prev = 0, cur = 0;
    idx_t t = n;
    while (t) {
        cur++;
        if (t & 1) {
            r++;
            if (r >= 2) ex[r] = cur - prev - 1;
            prev = cur;
        }
        t >>= 1;
    }
    if (r < 2) return (seq_t)1;
    ex[r] += 1;

    uint64_t v[70];
    int cnt = 0;
    for (int i = 2; i <= r; i++)
        if (ex[i] > 0) v[cnt++] = ex[i];

    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt; j++)
            if (v[i] < v[j]) { uint64_t x = v[i]; v[i] = v[j]; v[j] = x; }

    uint64_t pr[70];
    int pc = 0;
    for (uint64_t c = 2; pc < cnt; c++) {
        bool ok = true;
        for (int k = 0; k < pc; k++) {
            if (pr[k] * pr[k] > c) break;
            if (c % pr[k] == 0) { ok = false; break; }
        }
        if (ok) pr[pc++] = c;
    }

    seq_t res = 1;
    for (int i = 0; i < cnt; i++)
        res *= A365425_pow(pr[i], v[i]);
    return res;
}
