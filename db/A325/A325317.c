/*
 * A325317  a(n) = A048250(n) XOR A162296(n), where XOR is the bitwise-XOR,
 *          A003987.
 * https://oeis.org/A325317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048250(n) XOR A162296(n), where A048250(n) =
 *            Product_{p|n} (p+1) and A162296(n) = sigma(n) - A048250(n)
 * Range:     u8 n <= 107, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9494 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sigma_v = 1;
    seq_t sf = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pe = 1;
            while (m % p == 0) {
                m /= p;
                pe *= (seq_t)p;
                term += pe;
            }
            sigma_v *= term;
            sf *= ((seq_t)p + 1);
        }
    }
    if (m > 1) {
        sigma_v *= ((seq_t)m + 1);
        sf *= ((seq_t)m + 1);
    }

    seq_t b = sigma_v - sf;
    return sf ^ b;
}
