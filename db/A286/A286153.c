/*
 * A286153  Square array read by descending antidiagonals A(1,1), A(1,2),
 *          A(2,1), ...: If n > k, A(n,k) = T(n XOR k, k), and otherwise
 *          A(n,k) = T(n, n XOR k), where T(n,k) is sequence A001477
 *          considered as a two-dimensional table, and XOR is bitwise-xor
 *          (A003987).
 * https://oeis.org/A286153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = T(n XOR k, k) if n > k, else T(n, n XOR k), where
 *            T(x,y) = (x+y)*(x+y+1)/2 + x (A001477 read as a table); a(n) is
 *            A(n,k) read by descending antidiagonals, n,k >= 1
 * Range:     u8 n <= 97, u16 n <= 10585, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 107 terms through n = 9720 (2026-11-08)
 * Helpers:   isqrt_u128_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128_to_idx.h"

static seq_t A286153_T(idx_t x, idx_t y) {
    seq_t X = (seq_t)x, Y = (seq_t)y;
    seq_t s = X + Y;
    return s * (s + 1) / 2 + X;
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    unsigned __int128 v = (unsigned __int128)m * 8 + 1;
    idx_t r = isqrt_u128_to_idx(v);
    idx_t s = (r - 1) / 2;
    while ((unsigned __int128)s * (s + 1) / 2 > m) s--;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= m) s++;
    idx_t i = m - (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t row = i + 1;
    idx_t col = s - i + 1;
    idx_t x = row ^ col;
    if (row > col)
        return A286153_T(x, col);
    return A286153_T(row, x);
}
