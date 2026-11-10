/*
 * A257264  Square array A(row,col) read by antidiagonals: A(1,col) =
 *          A055938(col), and for row > 1, A(row,col) = A011371(A(row-1,col)).
 * https://oeis.org/A257264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(1,c) = A055938(c) (integers not of the form A005187(k) = 2k -
 *            popcount(k)); A(r,c) = A011371(A(r-1,c)) with A011371(m) = m -
 *            popcount(m); read by antidiagonals, so a(n) = A(k, d+1-k) where
 *            n = d(d-1)/2 + k, 1 <= k <= d
 * Range:     u8 n <= 8128, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 121 terms through n = 9625 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned pc_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) { c += (unsigned)(x & 1U); x >>= 1; }
    return c;
}

/* A055938(col): col-th positive integer not of form 2k - popcount(k) */
static uint64_t leaf_val(uint64_t col)
{
    uint64_t k = 0, b = 0, cnt = 0, m = 0;
    while (1) {
        m++;
        while (b < m) {
            k++;
            b = 2 * k - pc_u64(k);
        }
        if (b != m) {
            cnt++;
            if (cnt == col) return m;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t rem = (uint64_t)n, d = 1;
    while (rem > d) { rem -= d; d++; }
    uint64_t row = rem;
    uint64_t col = d + 1 - rem;
    uint64_t v = leaf_val(col);
    for (uint64_t i = 1; i < row && v != 0; i++)
        v -= pc_u64(v);
    return (seq_t)v;
}
