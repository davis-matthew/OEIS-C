/*
 * A325316  a(n) = A048250(n) OR A162296(n), where OR is the bitwise-OR,
 *          A003986.
 * https://oeis.org/A325316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048250(n) OR A162296(n), where A048250(n) =
 *            Product_{p|n} (p+1) and A162296(n) = sigma(n) - A048250(n)
 * Range:     u8 n <= 107, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9128 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    seq_t sigma = 1;
    seq_t sf = 1;

    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            seq_t term = 1;
            seq_t pe = 1;
            while (t % p == 0) {
                t /= p;
                pe *= (seq_t)p;
                term += pe;
            }
            sigma *= term;
            sf *= ((seq_t)p + 1);
        }
    }
    if (t > 1) {
        sigma *= ((seq_t)t + 1);
        sf *= ((seq_t)t + 1);
    }

    return (seq_t)(sf | (sigma - sf));
}
