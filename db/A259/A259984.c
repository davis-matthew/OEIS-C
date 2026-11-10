/*
 * A259984  a(n) = A000787(n) + 1.
 * https://oeis.org/A259984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000787(n) + 1, where A000787 lists the strobogrammatic
 *            numbers (built directly by mixed-radix digit construction)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A000787_rot(int d) {
    switch (d) {
        case 6: return 9;
        case 9: return 6;
        default: return d;
    }
}

static const int A000787_first[4] = {1, 6, 8, 9};
static const int A000787_inner[5] = {0, 1, 6, 8, 9};
static const int A000787_cen[3] = {0, 1, 8};

static seq_t A000787_a(idx_t n) {
    idx_t r = n - 1;
    if (r < 3) return (seq_t)A000787_cen[r];
    r -= 3;
    int L = 2;
    for (;;) {
        int h = L / 2;
        unsigned __int128 cnt = 4;
        for (int i = 1; i < h; i++) cnt *= 5;
        if (L % 2) cnt *= 3;
        if ((unsigned __int128)r < cnt) break;
        r -= (idx_t)cnt;
        L++;
    }
    int dig[160];
    int h = L / 2;
    idx_t k = r;
    if (L % 2) {
        dig[h] = A000787_cen[k % 3];
        k /= 3;
    }
    for (int i = h - 1; i >= 1; i--) {
        int d = A000787_inner[k % 5];
        k /= 5;
        dig[i] = d;
        dig[L - 1 - i] = A000787_rot(d);
    }
    dig[0] = A000787_first[k];
    dig[L - 1] = A000787_rot(dig[0]);
    seq_t v = 0;
    for (int i = 0; i < L; i++) v = v * 10 + dig[i];
    return v;
}

seq_t a(idx_t n) {
    return A000787_a(n) + 1;
}
