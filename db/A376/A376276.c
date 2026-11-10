/*
 * A376276  Table T(n, k) n > 0, k > 2 read by upward antidiagonals. The
 *          sequences in each column k is a triangle read by rows (blocks),
 *          where each row is a permutation of the numbers of its
 *          constituents. The length of the row number n in column k is equal
 *          to the n-th k-gonal number A086270.
 * https://oeis.org/A376276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) read by upward antidiagonals; column k is a triangle
 *            whose row m has length P_k(m) = ((k-2)m^2-(k-4)m)/2 (A086270)
 *            and is a permutation of the integers S(m-1)+1..S(m), where S(m)
 *            = (m^3(k-2)+3m^2-m(k-5))/6. Each row is ordered by distance from
 *            its center, upper element first on ties
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 130 terms through n = 9806 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long d = 1;
    while (d * (d + 1) / 2 < N) d++;
    long long i = N - d * (d - 1) / 2;   /* 1..d */
    long long r = d + 1 - i;             /* position within column */
    long long k = 2 + i;                 /* column */

    long long m = 0, prev = 0, cur = 0;
    for (;;) {
        m++;
        prev = cur;
        cur = (m * m * m * (k - 2) + 3 * m * m - m * (k - 5)) / 6;
        if (cur >= r) break;
    }
    long long L = cur - prev;
    long long j = r - prev - 1;          /* 0-based position in row */
    long long lo = prev + 1;
    long long v;
    if (L & 1) {
        long long c = lo + (L - 1) / 2;
        if (j == 0) v = c;
        else if (j & 1) v = c + (j + 1) / 2;
        else v = c - j / 2;
    } else {
        long long u = lo + L / 2;
        if (j == 0) v = u;
        else if (j & 1) v = u - (j + 1) / 2;
        else v = u + j / 2;
    }
    return (seq_t)v;
}
