/*
 * A309836  a(n) is the smallest k such that k/A002034(k) equals n, where
 *          A002034(k) is the k-th Kempner number.
 * https://oeis.org/A309836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*m for the smallest m >= 1 with A002034(n*m) = m (search
 *            over m)
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A309836_sp(seq_t p, seq_t e) {
    /* smallest x with p^e dividing x! */
    seq_t x = 0, t = 0;
    while (t < e) {
        x += p;
        seq_t y = x;
        while (y % p == 0) { y /= p; t++; }
    }
    return x;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    seq_t ps[130];
    seq_t es[130];
    int cnt = 0;
    seq_t t = n;
    for (seq_t p = 2; p <= t / p; p++) {
        if (t % p == 0) {
            seq_t e = 0;
            while (t % p == 0) { t /= p; e++; }
            ps[cnt] = p; es[cnt] = e; cnt++;
        }
    }
    if (t > 1) { ps[cnt] = t; es[cnt] = 1; cnt++; }

    for (seq_t m = 1;; m++) {
        seq_t r = m;
        seq_t best = 1;
        for (int i = 0; i < cnt; i++) {
            seq_t e = es[i];
            while (r % ps[i] == 0) { r /= ps[i]; e++; }
            seq_t s = A309836_sp(ps[i], e);
            if (s > best) best = s;
        }
        for (seq_t p = 2; p <= r / p; p++) {
            if (r % p == 0) {
                seq_t e = 0;
                while (r % p == 0) { r /= p; e++; }
                seq_t s = A309836_sp(p, e);
                if (s > best) best = s;
            }
        }
        if (r > 1) {
            if (r > best) best = r;
        }
        if (best == m) return n * m;
    }
}
