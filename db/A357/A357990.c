/*
 * A357990  Square array T(n, k), n >= 0, k > 0, read by antidiagonals, where
 *          T(n, k) = R(n, k+1) - R(n, k) for n > 0, k > 0 with T(0, k) = 1
 *          for k > 0 and where R(n, k) = T(A053645(n), k)*k^(A290255(n) + 1).
 * https://oeis.org/A357990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   T(0,k)=1; T(n,k)=R(n,k+1)-R(n,k),
 *            R(n,k)=T(A053645(n),k)*k^(A290255(n)+1); a(idx) = T(n,k) read by
 *            antidiagonals (n descending from d, k = d-n+1)
 * Range:     u8 n <= 68, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A357990_msb(idx_t x)
{
    idx_t r = 0;
    while (x > 1) { x >>= 1; ++r; }
    return r;
}

static seq_t A357990_T(idx_t n, seq_t k)
{
    if (n == 0) return (seq_t)1;
    idx_t msb = A357990_msb(n);
    idx_t high = ((idx_t)1) << msb;
    idx_t m = n - high;                      /* A053645(n) */
    idx_t e;                                 /* A290255(n) */
    if (m == 0) e = msb;
    else e = msb - A357990_msb(m) - 1;

    seq_t t1 = A357990_T(m, k + 1);
    seq_t t0 = A357990_T(m, k);
    seq_t p1 = (seq_t)1, p0 = (seq_t)1;
    seq_t k1 = k + 1;
    for (idx_t i = 0; i <= e; ++i) {
        p1 = p1 * k1;
        p0 = p0 * k;
    }
    return t1 * p1 - t0 * p0;
}

seq_t a(idx_t n)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    idx_t pos = n - d * (d + 1) / 2;
    idx_t row = d - pos;
    seq_t k = (seq_t)pos + 1;
    return A357990_T(row, k);
}
