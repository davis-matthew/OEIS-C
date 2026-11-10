/*
 * A304044  Numbers k such that A000700(k) is divisible by k.
 * https://oeis.org/A304044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search for k with A000700(k) == 0 (mod k). A000700 is computed
 *            exactly in multiword integers via A(q)*(q;q)_inf = phi(-q^2),
 *            i.e. pentagonal-number recurrence A(n) = sum_{j>=1} (-1)^(j+1)
 *            (A(n-j(3j-1)/2) + A(n-j(3j+1)/2)) + [n=2m^2](-1)^m*2, A(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SC_STRIDE 11
#define SC_LIMIT 100000

static uint64_t *sc_tab = 0;
static idx_t sc_done = 0;      /* A(0..sc_done-1) computed and tested */
static idx_t sc_terms[64];
static int sc_cnt = 0;

static void sc_step(void) {
    idx_t n = sc_done;
    uint64_t pos[SC_STRIDE], neg[SC_STRIDE];
    int L = (int)((3.1416 * 1.4427 * sqrt((double)n / 6.0) + 14.0) / 64.0) + 1;
    if (L > SC_STRIDE) L = SC_STRIDE;
    int i;
    for (i = 0; i < SC_STRIDE; i++) { pos[i] = 0; neg[i] = 0; }
    if (n == 0) {
        sc_tab[0] = 1;
        sc_done = 1;
        return;
    }
    for (idx_t j = 1;; j++) {
        idx_t g1 = j * (3 * j - 1) / 2;
        if (g1 > n) break;
        uint64_t *acc = (j & 1) ? pos : neg;
        for (int t = 0; t < 2; t++) {
            idx_t g = t ? j * (3 * j + 1) / 2 : g1;
            if (g > n) continue;
            const uint64_t *src = sc_tab + (size_t)(n - g) * SC_STRIDE;
            unsigned carry = 0;
            for (i = 0; i < L; i++) {
                uint64_t s = acc[i] + src[i];
                unsigned c1 = s < acc[i];
                uint64_t s2 = s + carry;
                unsigned c2 = s2 < s;
                acc[i] = s2;
                carry = c1 | c2;
            }
        }
    }
    /* phi(-q^2) term */
    if (n % 2 == 0) {
        idx_t h = n / 2;
        idx_t m = (idx_t)sqrt((double)h);
        while (m * m > h) m--;
        while ((m + 1) * (m + 1) <= h) m++;
        if (m * m == h && m > 0) {
            uint64_t *acc = (m & 1) ? neg : pos;
            unsigned carry = 2;
            for (i = 0; i < L && carry; i++) {
                uint64_t s = acc[i] + carry;
                carry = s < acc[i];
                acc[i] = s;
            }
        }
    }
    uint64_t *dst = sc_tab + (size_t)n * SC_STRIDE;
    unsigned borrow = 0;
    for (i = 0; i < SC_STRIDE; i++) {
        uint64_t x = pos[i], y = neg[i];
        uint64_t d = x - y;
        unsigned b1 = x < y;
        uint64_t d2 = d - borrow;
        unsigned b2 = d < borrow;
        dst[i] = d2;
        borrow = b1 | b2;
    }
    /* test divisibility */
    unsigned __int128 r = 0;
    for (i = L - 1; i >= 0; i--) {
        r = ((r << 64) | dst[i]) % (unsigned __int128)n;
    }
    if (r == 0 && sc_cnt < 64) sc_terms[sc_cnt++] = n;
    sc_done = n + 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!sc_tab) {
        sc_tab = (uint64_t *)calloc((size_t)(SC_LIMIT + 1) * SC_STRIDE, sizeof(uint64_t));
        if (!sc_tab) return 0;
    }
    while ((idx_t)sc_cnt < n && sc_done <= SC_LIMIT && sc_cnt < 64) sc_step();
    if ((idx_t)sc_cnt >= n) return (seq_t)sc_terms[n - 1];
    return 0;
}
