/*
 * A338947  Number of vertices of a hexagonal tessellation that lie on
 *          subsequent circles centered at a vertex of one hexagon.
 * https://oeis.org/A338947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n)=r(m)/2 if 3 does not divide m, else r(m), where
 *            m=A003136(n) (a(0)=1 for m=0) and
 *            r(m)=6*(d_{1,3}(m)-d_{2,3}(m)); found by search over m
 * Range:     u8 n <= 81, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 97 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A338947_count(seq_t m) {
    seq_t prod = 1;
    seq_t n = m;
    for (seq_t p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            seq_t e = 0;
            while (n % p == 0) { n /= p; e++; }
            if (p == 3) continue;
            if (p % 3 == 1) prod *= (e + 1);
            else if (e % 2 != 0) return 0;
        }
    }
    if (n > 1) {
        if (n % 3 == 1) prod *= 2;
        else if (n % 3 == 2) return 0;
    }
    if (m % 3 == 0) return 6 * prod;
    return 3 * prod;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t k = 0;
    seq_t v = 1;
    while (1) {
        seq_t c = A338947_count(v);
        if (c != 0) {
            k++;
            if (k == n) return c;
        }
        v++;
    }
}
