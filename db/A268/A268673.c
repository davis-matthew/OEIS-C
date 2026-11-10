/*
 * A268673  a(0) = 0; a(1) = 1; for n > 1, a(n) = 1 + 4*A092246(n-1).
 * https://oeis.org/A268673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; for n > 1, a(n) = 1 + 4*A092246(n-1), where
 *            A092246 = odd numbers with odd binary popcount (computed by
 *            search)
 * Range:     u8 n <= 17, u16 n <= 4097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9659 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int A268673_parity(seq_t x)
{
    int p = 0;
    while (x != 0) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    idx_t k = n - 1;
    idx_t count = 0;
    seq_t c = 1;
    for (;;) {
        if (A268673_parity(c)) {
            count++;
            if (count == k) break;
        }
        c += 2;
    }
    return 1 + 4 * c;
}
