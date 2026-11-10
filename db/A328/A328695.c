/*
 * A328695  Rectangular array R read by descending antidiagonals: divide to
 *          each even term of the Wythoff array (A035513) by 2, and delete all
 *          others.
 * https://oeis.org/A328695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (Wythoff array entry)/2 for the even entries, read by
 *            descending antidiagonals; R(r,c) = c-th even term of Wythoff row
 *            r; Wythoff row r term k (k>=0) = F(k+2)*floor(r*phi) +
 *            F(k+1)*(r-1)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A328695_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967296ULL;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) d++;
    idx_t p = n - (d * (d - 1)) / 2;   /* 1..d */
    idx_t row = p;
    idx_t col = d + 1 - p;

    uint64_t r = (uint64_t)row;
    uint64_t fl = (r + A328695_isqrt(5 * r * r)) / 2;   /* floor(r*phi) */
    uint64_t rm1 = r - 1;

    /* parities of first terms of Wythoff row */
    unsigned x0 = (unsigned)((fl + rm1) & 1);
    unsigned x1 = (unsigned)(rm1 & 1);
    idx_t j, step;
    if (x0 == 0 && x1 == 0) { j = 0; step = 1; }
    else if (x0 == 0) { j = 0; step = 3; }
    else if (x1 == 0) { j = 1; step = 3; }
    else { j = 2; step = 3; }

    idx_t k = j + step * (col - 1);

    seq_t fa = 1, fb = 1;   /* F(1), F(2) */
    for (idx_t i = 0; i < k; i++) {
        seq_t t = fa + fb;
        fa = fb;
        fb = t;
    }
    /* fa = F(k+1), fb = F(k+2) */
    seq_t A = fb * (seq_t)fl;
    seq_t B = fa * (seq_t)rm1;
    seq_t res = A / 2 + B / 2 + (A & B & 1);
    return res;
}
