/*
 * A286956  Main diagonal of A286950.
 * https://oeis.org/A286956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010815(n) + n, where A010815(n) = (-1)^m if n =
 *            m(3m±1)/2, else 0 (a(0)=1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 891 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A010815_sign(idx_t n) {
    if (n == 0) return 1;
    for (idx_t m = 1; ; ++m) {
        unsigned __int128 mm = m;
        unsigned __int128 g1 = mm * (((unsigned __int128)3 * mm) - 1) / 2;
        unsigned __int128 g2 = mm * (((unsigned __int128)3 * mm) + 1) / 2;
        unsigned __int128 un = n;
        if (g1 > un && g2 > un) break;
        if (g1 == un || g2 == un) return (m & 1) ? -1 : 1;
    }
    return 0;
}

seq_t a(idx_t n) {
    int e = A010815_sign(n);
    seq_t r = (seq_t)n;
    if (e == 1) r = r + 1;
    else if (e == -1) r = r - 1;
    return r;
}
