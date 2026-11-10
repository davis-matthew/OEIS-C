/*
 * A268687  a(n) = MAX(g_k(n)) where g_k(n) is the function defined in
 *          A266202.
 * https://oeis.org/A268687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = max_{k>=0} g_k(n), g_0(n)=n, g_k(n) = b_{k+1}(g_{k-1}(n))
 *            - 1 (weak Goodstein, base k+1 -> k+2); computed by iterating
 *            until the value reaches 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    seq_t best = val;
    seq_t base = 2;
    while (val > 0) {
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
        if (val > best) best = val;
        base += 1;
    }
    return best;
}
