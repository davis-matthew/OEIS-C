/*
 * A269159  Transpose of A269158.
 * https://oeis.org/A269159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(c,r) where A269158 has A(row,col)=F(row,2*col-1),
 *            F(p,q)=0 for p<=1, F(p,q)=F(q mod p,p) XOR (p AND q) for p odd,
 *            F(p,q)=F(p/2,q) XOR Gray(q) for p even; the transpose is read by
 *            descending antidiagonals
 * Range:     u8 n <= 8256, u16 n <= 8256, u32 n <= 8256, u64 n <= 8256,
 *            u128 n <= 8256, big any n
 * Validated: standard, pass, 121 terms through n = 8121 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A269159_f(seq_t p, seq_t q)
{
    seq_t r = 0;
    while (p > 1) {
        if (p & 1) {
            seq_t m = q % p;
            r ^= (p & q);
            q = p;
            p = m;
        } else {
            r ^= q ^ (q >> 1);
            p >>= 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t S = 1;
    while ((S * (S + 1)) / 2 < N) S++;
    uint64_t prev = (S * (S - 1)) / 2;
    uint64_t pos = N - prev - 1;
    uint64_t row = pos + 1;
    uint64_t col = S - row + 1;
    /* transpose: swap roles of row and col */
    seq_t p = (seq_t)col;
    seq_t q = (seq_t)2 * (seq_t)row - 1;
    return A269159_f(p, q);
}
