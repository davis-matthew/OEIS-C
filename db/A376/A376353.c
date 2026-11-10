/*
 * A376353  Table T(n, k) n > 0, k > 2 read by upward antidiagonals. The
 *          sequences in each column k is a triangle read by rows (blocks),
 *          where each row is a permutation of the numbers of its
 *          constituents. The length of the row number n in column k is equal
 *          to the n-th k-pyramidal number A261720.
 * https://oeis.org/A376353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) read by upward antidiagonals (k>=3). Column k is split
 *            into rows j of length len(j)=j*(j+1)*((k-2)*(j-1)+3)/6
 *            (k-pyramidal numbers); with S(j-1)=sum of earlier lengths, row j
 *            lists S(j-1)+v where v runs through the outside-in order
 *            1,L,2,L-1,3,... of 1..L reversed
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 128 terms through n = 9317 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* locate (row r, col c) on upward antidiagonals */
    idx_t s = 1;
    while ((idx_t)(s * (s - 1) / 2) < n) s++;
    idx_t start = (s - 1) * (s - 2) / 2 + 1;
    idx_t off = n - start;
    idx_t r = s - 1 - off;
    idx_t k = 3 + off;

    seq_t pos = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t prev = 0;      /* S(j-1) */
    seq_t len = 0;
    seq_t j = 1;
    for (;;) {
        len = j * (j + 1) * ((kk - 2) * (j - 1) + 3) / 6;
        if (prev + len >= pos) break;
        prev += len;
        j++;
    }
    seq_t p = pos - prev;
    seq_t q = len - p + 1;
    seq_t v;
    if ((q & 1) == 1) v = (q + 1) / 2;
    else v = len - q / 2 + 1;
    return prev + v;
}
