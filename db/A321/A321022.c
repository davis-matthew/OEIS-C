/*
 * A321022  The 100 terms of the cycle that A321021 goes into.
 * https://oeis.org/A321022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-2)+a(n-1) keeping only digits that appear exactly
 *            once, a(1)=7, a(2)=0; the sequence is periodic with period 100,
 *            so a(n) = a(((n-1) mod 100)+1)
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A321022_keep_unique(seq_t s)
{
    int counts[10] = {0};
    int rev[64];
    int len = 0;
    seq_t t = s;
    if (t == 0) {
        counts[0] = 1;
        rev[len++] = 0;
    } else {
        while (t > 0 && len < 64) {
            int d = (int)(t % 10);
            counts[d]++;
            rev[len++] = d;
            t /= 10;
        }
    }
    seq_t res = 0;
    for (int j = len - 1; j >= 0; --j) {
        if (counts[rev[j]] == 1) {
            res = res * 10 + (seq_t)rev[j];
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t tab[101];
    static int ready = 0;
    if (!ready) {
        tab[1] = 7;
        tab[2] = 0;
        for (int i = 3; i <= 100; ++i) {
            tab[i] = A321022_keep_unique(tab[i - 2] + tab[i - 1]);
        }
        ready = 1;
    }
    if (n == 0) n = 100;
    int idx = (int)((n - 1) % 100) + 1;
    return tab[idx];
}
