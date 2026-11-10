/*
 * A285112  Permutation of natural numbers: a(0) = 1, a(1) = 2, a(2n) =
 *          A005117(1+a(n)), a(2n+1) = A065642(a(n)).
 * https://oeis.org/A285112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(1) = 2, a(2n) = A005117(1+a(n)), a(2n+1) =
 *            A065642(a(n)); computed by walking the binary digits of n from
 *            the top, using a search for the k-th squarefree number and a
 *            search for the next number with the same radical
 * Range:     u8 n <= 34, u16 n <= 116, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Validated: standard, pass, 97 terms through n = 112 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A285112_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static bool A285112_sqfree(seq_t k) {
    if (k % 4 == 0) return false;
    for (seq_t i = 3; i * i <= k; i += 2) {
        if (k % (i * i) == 0) return false;
    }
    return true;
}

/* A005117(k): k-th squarefree number */
static seq_t A285112_A005117(seq_t k) {
    seq_t c = 0;
    seq_t cnt = 0;
    while (cnt < k) {
        c++;
        if (A285112_sqfree(c)) cnt++;
    }
    return c;
}

static seq_t A285112_radical(seq_t m) {
    seq_t r = 1;
    if (m % 2 == 0) {
        r = 2;
        while (m % 2 == 0) m /= 2;
    }
    for (seq_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            r *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= m;
    return r;
}

/* A065642(v) for v >= 2 */
static seq_t A285112_A065642(seq_t v) {
    if (v == 1) return 1;
    seq_t r = A285112_radical(v);
    seq_t cand = (v / r + 1) * r;
    for (;; cand += r) {
        seq_t t = cand;
        seq_t g = A285112_gcd(t, r);
        while (g > 1) {
            t /= g;
            g = A285112_gcd(t, r);
        }
        if (t == 1) return cand;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    int top = 0;
    for (idx_t m = n; m > 1; m >>= 1) top++;
    seq_t v = 2;
    for (int i = top - 1; i >= 0; i--) {
        if ((n >> i) & 1) {
            v = A285112_A065642(v);
        } else {
            v = A285112_A005117(v + 1);
        }
    }
    return v;
}
