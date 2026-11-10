/*
 * A267648  a(n) = g_n(5) where g is the function defined in A266202.
 * https://oeis.org/A267648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = g_n(5), g_0(5)=5, g_k(5) = b_{k+1}(g_{k-1}(5)) - 1
 *            (hereditary-free weak Goodstein: rewrite base-(k+1) digits in
 *            base k+2... i.e. base k+1 -> k+2), stays 0 once reached
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 5;
    for (idx_t k = 1; k <= n; ++k) {
        if (val == 0) break;
        seq_t base = (seq_t)k + 1;
        seq_t tmp = val;
        seq_t nv = 0;
        seq_t pw = 1;
        while (tmp > 0) {
            seq_t d = tmp % base;
            nv += d * pw;
            pw *= (base + 1);
            tmp /= base;
        }
        val = nv - 1;
    }
    return val;
}
