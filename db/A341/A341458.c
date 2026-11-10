/*
 * A341458  Unique square array T(n, k) read by antidiagonals, n, k > 0, such
 *          that A000069(T(n, k)) = A341288(A000069(n), A000069(k)).
 * https://oeis.org/A341458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   T(n,k) = A000069^{-1}( A341288(A000069(n), A000069(k)) ), where
 *            A000069^{-1}(m) = floor(m/2)+1; the array is read by
 *            antidiagonals
 * Range:     u8 n <= 8384, u16 n <= 8385, u32 n <= 8385, u64 n <= 8385,
 *            u128 n <= 8385, big any n
 * Validated: standard, pass, 94 terms through n = 8302 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A000069_odious(idx_t n)
{
    idx_t m = n - 1;
    idx_t x = m;
    int parity = 0;
    while (x) {
        parity ^= (int)(x & 1);
        x >>= 1;
    }
    return m * 2 + (parity ? 0 : 1);
}

static seq_t A341288_prod(idx_t a, idx_t b)
{
    seq_t res = 0;
    for (idx_t ub = a; ub; ub &= ub - 1) {
        idx_t u = (idx_t)__builtin_ctzll((unsigned long long)ub);
        for (idx_t vb = b; vb; vb &= vb - 1) {
            idx_t v = (idx_t)__builtin_ctzll((unsigned long long)vb);
            seq_t bit = (seq_t)1 << (int)(u ^ v);
            res ^= bit;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        d++;
    idx_t i = n - d * (d - 1) / 2;
    idx_t r = i;
    idx_t c = d + 1 - i;
    seq_t m = A341288_prod(A000069_odious(r), A000069_odious(c));
    return (m >> 1) + 1;
}
