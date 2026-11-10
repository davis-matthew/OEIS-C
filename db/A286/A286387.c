/*
 * A286387  a(n) = A002487(n^2).
 * https://oeis.org/A286387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(n^2), where A002487 is Stern's diatomic series:
 *            a(0)=0, a(1)=1, a(2k)=a(k), a(2k+1)=a(k)+a(k+1)
 * Range:     u8 n <= 98, u16 n <= 5260, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 107 terms through n = 7417 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A286387_stern(unsigned __int128 m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)n * (unsigned __int128)n;
    return A286387_stern(m);
}
