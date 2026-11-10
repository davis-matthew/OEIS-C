/*
 * A243623  Prime terms in A214829.
 * https://oeis.org/A243623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th distinct prime among the terms of A214829, *
 *            where A214829(k) = A214829(k-1) + A214829(k-2) + A214829(k-3), *
 *            A214829(0) = 1, A214829(1) = A214829(2) = 7. * Primality is
 *            tested by trial division and then by Miller-Rabin
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   mem, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/mulmod_seq.h"

static int A243623_is_bounded(void) {
    static int r = -1;
    if (r < 0) {
        seq_t t = (seq_t)1;
        for (int i = 0; i < 200; i++) t += t;
        r = (t == (seq_t)0) ? 1 : 0;
    }
    return r;
}

static seq_t A243623_mulmod(seq_t x, seq_t y, seq_t m) {
    if (A243623_is_bounded()) return mulmod_seq(x, y, m);
    return (x * y) % m;
}

static seq_t A243623_powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = (seq_t)1 % m;
    b = b % m;
    while (e > (seq_t)0) {
        if ((e & (seq_t)1) != (seq_t)0) r = A243623_mulmod(r, b, m);
        e >>= 1;
        if (e > (seq_t)0) b = A243623_mulmod(b, b, m);
    }
    return r;
}

static bool A243623_is_prime(seq_t m) {
    static const int sp[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    const int nsp = 15;
    if (m < (seq_t)2) return false;
    for (int i = 0; i < nsp; i++) {
        if (m == (seq_t)sp[i]) return true;
        if (m % (seq_t)sp[i] == (seq_t)0) return false;
    }
    if (m < (seq_t)2209) return true; /* 47^2 */
    /* further trial division by small odd numbers */
    for (int q = 53; q < 1000; q += 2) {
        if (m < (seq_t)q * (seq_t)q) return true;
        if (m % (seq_t)q == (seq_t)0) return false;
    }
    seq_t d = m - (seq_t)1;
    int s = 0;
    while ((d & (seq_t)1) == (seq_t)0) { d >>= 1; s++; }
    seq_t mm1 = m - (seq_t)1;
    for (int i = 0; i < 13; i++) {
        seq_t x = A243623_powmod((seq_t)sp[i], d, m);
        if (x == (seq_t)1 || x == mm1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A243623_mulmod(x, x, m);
            if (x == mm1) { comp = false; break; }
            if (x == (seq_t)1) break;
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *found = NULL;
    static idx_t cnt = 0, cap = 0;
    static bool init = false, overflow = false;
    static seq_t x, y, z;   /* three most recent terms of A214829 */

    if (!init) {
        init = true;
        x = (seq_t)1; y = (seq_t)7; z = (seq_t)7;
        cap = 16;
        found = mem_new(seq_t, cap);
        found[0] = (seq_t)7;   /* 7 appears twice in A214829, listed once */
        cnt = 1;
    }
    while (cnt < n && !overflow) {
        seq_t s1 = x + y;
        if (s1 < x) { overflow = true; break; }
        seq_t s2 = s1 + z;
        if (s2 < s1) { overflow = true; break; }
        x = y; y = z; z = s2;
        if (A243623_is_prime(s2)) {
            if (cnt >= cap) {
                seq_t *nf = mem_resize(seq_t, found, cap * 2);
                if (nf == NULL) { overflow = true; break; }
                found = nf;
                cap *= 2;
            }
            found[cnt++] = s2;
        }
    }
    if (n >= 1 && n <= cnt) return found[n - 1];
    return (seq_t)0;
}
