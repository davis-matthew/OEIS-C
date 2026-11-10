/*
 * A360134  a(n) = A356133(1 + A026430(n)).
 * https://oeis.org/A360134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A356133(1 + A026430(n)) = 3*m - 1 - parity_of_ones(m-1),
 *            m = 1 + s, where s = A026430(n) = n + floor(n/2) + (n odd ?
 *            t(n-1) : 0), t = Thue-Morse
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A360134_parity(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t s = n + n / 2;
    if (n & 1) s += (idx_t)A360134_parity(n - 1);   /* A026430(n) */
    int p = A360134_parity(s);                      /* parity of (m-1) with m = s+1 */
    seq_t m = (seq_t)s + 1;
    seq_t res = (seq_t)3 * m;
    res -= (seq_t)1;
    res -= (seq_t)p;
    return res;
}
