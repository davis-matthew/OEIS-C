/*
 * A260480  Positions of 0 in the infinite palindromic word at A260455.
 * https://oeis.org/A260480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the palindromic word A260455
 *            (found by scanning positions)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9681 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A260455_bit(idx_t n)
{
    idx_t len[70];
    int k = 1;
    len[1] = 1;
    while (len[k] < n) {
        ++k;
        if ((k & 1) == 0) len[k] = 2 * len[k-1] + 1;
        else              len[k] = 2 * len[k-1];
    }
    idx_t pos = n;
    while (k > 1) {
        idx_t left = len[k-1];
        if ((k & 1) == 0) {
            if (pos == left + 1) return 1;
            if (pos > left + 1) pos = 2 * left + 2 - pos;
        } else {
            if (pos > left) pos = 2 * left + 1 - pos;
        }
        --k;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0, p = 0;
    while (cnt < n) {
        ++p;
        if (A260455_bit(p)) ++cnt;
    }
    return (seq_t)p;
}
