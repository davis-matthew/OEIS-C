/*
 * A372313  Table read by antidiagonals: A(n,1) = 2n-1, and for k > 1, A(n,k)
 *          = A372289(A(n,k-1)+A(n,1)).
 * https://oeis.org/A372313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,1) = 2n-1, A(n,k) = A372289(A(n,k-1)+A(n,1)), where
 *            A372289(m) = m*2^e + (4^e-1)/3, e = v2(m); table read by
 *            antidiagonals: a(1)=A(1,1), a(2)=A(1,2), a(3)=A(2,1),
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A372289_a(seq_t m) {
    unsigned e = 0;
    seq_t tmp = m;
    while ((tmp & 1) == 0) {
        e++;
        tmp >>= 1;
    }
    seq_t p2 = 1, p4 = 1;
    for (unsigned i = 0; i < e; ++i) {
        p2 *= 2;
        p4 *= 4;
    }
    return m * p2 + (p4 - 1) / 3;
}

seq_t a(idx_t n) {
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;
    idx_t row = n - d * (d - 1) / 2;
    idx_t col = d + 1 - row;
    seq_t base = (seq_t)row * 2 - 1;
    seq_t x = base;
    for (idx_t k = 2; k <= col; ++k) {
        x = A372289_a(x + base);
    }
    return x;
}
