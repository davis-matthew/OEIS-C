/*
 * A300718  Möbius transform of A010848, number of numbers k <= n such that at
 *          least one prime factor of n is not a prime factor of k.
 * https://oeis.org/A300718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(n) - A300717(n), where A300717 is multiplicative
 *            with a(p)=0, a(p^e)=(p-1)*p^(e-2) for e>=2
 * Range:     u8 n <= 256, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9824 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t phi = 1, b = 1;
    int bzero = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            seq_t sp = (seq_t)p;
            seq_t pw = 1;
            for (int i = 0; i < e - 2; i++) pw *= sp;
            if (e == 1) {
                phi *= (sp - 1);
                bzero = 1;
            } else {
                phi *= (sp - 1) * pw * sp;
                b *= (sp - 1) * pw;
            }
        }
    }
    if (m > 1) {
        phi *= ((seq_t)m - 1);
        bzero = 1;
    }
    if (bzero) b = 0;
    return phi - b;
}
