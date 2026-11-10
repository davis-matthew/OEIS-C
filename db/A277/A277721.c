/*
 * A277721  a(n) = floor(n*t^3) - A003146(n), where t = 1.8392867... is the
 *          tribonacci constant A058265.
 * https://oeis.org/A277721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(n*t^3) - A003146(n), t = tribonacci constant. u =
 *            t^3 is the real root of x^3 - 7x^2 + 5x - 1; floor(n*u) is found
 *            by exact integer binary search on the sign of n^3*p(m/n).
 *            A003146(n) is the position of the n-th 'c' in the tribonacci
 *            word
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 2, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8768 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static struct {
    seq_t len, cnt;
} A003146_seg[100];

static void A003146_init_seg(void) {
    A003146_seg[0].len = 1; A003146_seg[0].cnt = 0;
    A003146_seg[1].len = 2; A003146_seg[1].cnt = 0;
    A003146_seg[2].len = 4; A003146_seg[2].cnt = 1;
    for (int i = 3; i < 100; i++) {
        A003146_seg[i].len = A003146_seg[i-1].len + A003146_seg[i-2].len + A003146_seg[i-3].len;
        A003146_seg[i].cnt = A003146_seg[i-1].cnt + A003146_seg[i-2].cnt + A003146_seg[i-3].cnt;
        if (A003146_seg[i].len > ((seq_t)1 << 62)) break;
    }
}

static seq_t A003146_find_nth(int si, seq_t n) {
    if (si <= 1) return 0;
    if (si == 2) return 4;
    seq_t c1 = A003146_seg[si-1].cnt;
    seq_t c2 = A003146_seg[si-2].cnt;
    if (n <= c1) {
        return A003146_find_nth(si-1, n);
    } else if (n <= c1 + c2) {
        return A003146_seg[si-1].len + A003146_find_nth(si-2, n - c1);
    } else {
        return A003146_seg[si-1].len + A003146_seg[si-2].len + A003146_find_nth(si-3, n - c1 - c2);
    }
}

static seq_t A003146_a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        A003146_init_seg();
        initialized = true;
    }
    int si = 2;
    seq_t nn = (seq_t)n;
    while (si < 100 && A003146_seg[si].cnt < nn) {
        si++;
    }
    return A003146_find_nth(si, nn);
}

/* sign of n^3 * p(m/n), p(x)=x^3-7x^2+5x-1 */
static seq_t A277721_poly(seq_t m, seq_t n) {
    return m*m*m - 7*m*m*n + 5*m*n*n - n*n*n;
}

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t lo = 6 * nn;       /* p(m/n) < 0 here: m/n=6 < u */
    seq_t hi = 7 * nn;       /* p > 0 here */
    while (hi - lo > 1) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A277721_poly(mid, nn) < 0) lo = mid;
        else hi = mid;
    }
    return lo - A003146_a(n);
}
