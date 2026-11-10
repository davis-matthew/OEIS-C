/*
 * A285347  Positions of 1 in A285345; complement of A285346.
 * https://oeis.org/A285347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n - 1 - A189668(n), where A189668 is the fixed point of
 *            0->010, 1->100, with b(3k-2) = b(k), b(3k-1) = 1 - b(k), b(3k) =
 *            0, b(1) = 0, b(2) = 1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8726 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A285347_b(idx_t n)
{
    unsigned flip = 0;
    unsigned val;
    idx_t m = n;
    for (;;) {
        if (m <= 2) {
            val = (m == 2) ? 1u : 0u;
            break;
        }
        idx_t r = m % 3;
        if (r == 0) {
            val = 0;
            break;
        } else if (r == 1) {
            m = (m + 2) / 3;
        } else {
            flip ^= 1u;
            m = (m + 1) / 3;
        }
    }
    return flip ? 1u - val : val;
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n * 2 - 1;
    unsigned b = A285347_b(n);
    if (b) r = r - 1;
    return r;
}
