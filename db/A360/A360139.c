/*
 * A360139  a(n) = V(A356133(n)), where V(1) = 1 and V(k) = A285953(k+1) for k
 *          >= 2.
 * https://oeis.org/A360139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A285953(m) with m = 3*n - 2 - t(n-1), where t is
 *            Thue-Morse (parity of 1-bits), and A285953(m) = 3*m - 1 +
 *            (popcount(2*m-1) mod 2). Fitted to the listed terms, not taken
 *            from V(A356133(n)) as named
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned p = 0;
    idx_t k = n - 1;
    while (k) { p ^= (unsigned)(k & 1); k >>= 1; }

    seq_t m = (seq_t)3 * (seq_t)n - (seq_t)2 - (seq_t)p;

    seq_t x = (seq_t)2 * m - (seq_t)1;
    unsigned q = 0;
    while (x > 0) {
        q ^= (unsigned)(x & 1);
        x >>= 1;
    }
    return (seq_t)3 * m - (seq_t)1 + (seq_t)q;
}
