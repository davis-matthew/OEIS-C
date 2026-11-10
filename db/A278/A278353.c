/*
 * A278353  Numbers n such that A003146(n) = floor(alpha^3*n)+1, where alpha =
 *          1.839... is the positive real zero of x^3-x^2-x-1.
 * https://oeis.org/A278353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = k-th n >= 1 with A003146(n) = floor(alpha^3*n)+1; found
 *            by search, floor(alpha^3*n) via exact integer test with minimal
 *            polynomial x^3-7x^2+5x-1 of alpha^3
 * Range:     u8 none, u16 none, u32 none, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static struct {
    seq_t len, cnt;
} A278353_seg[100];

static void A278353_init_seg(void) {
    A278353_seg[0].len = 1; A278353_seg[0].cnt = 0;
    A278353_seg[1].len = 2; A278353_seg[1].cnt = 0;
    A278353_seg[2].len = 4; A278353_seg[2].cnt = 1;
    for (int i = 3; i < 100; i++) {
        A278353_seg[i].len = A278353_seg[i-1].len + A278353_seg[i-2].len + A278353_seg[i-3].len;
        A278353_seg[i].cnt = A278353_seg[i-1].cnt + A278353_seg[i-2].cnt + A278353_seg[i-3].cnt;
        if (A278353_seg[i].len > ((seq_t)1 << 62)) break;
    }
}

static seq_t A278353_find_nth(int si, seq_t n) {
    if (si <= 1) return 0;
    if (si == 2) return 4;
    seq_t c1 = A278353_seg[si-1].cnt;
    seq_t c2 = A278353_seg[si-2].cnt;
    if (n <= c1) {
        return A278353_find_nth(si-1, n);
    } else if (n <= c1 + c2) {
        return A278353_seg[si-1].len + A278353_find_nth(si-2, n - c1);
    } else {
        return A278353_seg[si-1].len + A278353_seg[si-2].len + A278353_find_nth(si-3, n - c1 - c2);
    }
}

static seq_t A003146_a(seq_t nn) {
    static bool initialized = false;
    if (!initialized) {
        A278353_init_seg();
        initialized = true;
    }
    int si = 2;
    while (si < 100 && A278353_seg[si].cnt < nn) si++;
    return A278353_find_nth(si, nn);
}

/* true iff m <= alpha^3 * n, i.e. m^3 - 7 m^2 n + 5 m n^2 - n^3 <= 0 (m/n > 4.2) */
static bool A278353_le(seq_t m, seq_t n) {
    seq_t pos = m * m * m + (seq_t)5 * m * n * n;
    seq_t neg = (seq_t)7 * m * m * n + n * n * n;
    return pos <= neg;
}

static seq_t A278353_floor_beta(seq_t n) {
    seq_t m = n * 6222262 / 1000000;
    while (A278353_le(m + 1, n)) m++;
    while (m > 0 && !A278353_le(m, n)) m--;
    return m;
}

seq_t a(idx_t k) {
    static idx_t last_k = 0;
    static seq_t last_n = 0;
    if (last_k == 0 || k < last_k) {
        last_k = 0;
        last_n = 0;
    }
    idx_t cnt = last_k;
    seq_t n = last_n;
    while (cnt < k) {
        n++;
        if (A003146_a(n) == A278353_floor_beta(n) + 1) cnt++;
    }
    last_k = cnt;
    last_n = n;
    return n;
}
