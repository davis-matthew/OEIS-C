/*
 * A325318  a(n) = A048250(n) AND A162296(n), where AND is the bitwise-AND,
 *          A004198.
 * https://oeis.org/A325318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048250(n) AND A162296(n), where A048250(n) =
 *            Product_{p|n} (p+1) and A162296(n) = sigma(n) - A048250(n)
 * Range:     u8 n <= 275, u16 n <= 47459, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9482 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sigma = 1;
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
            sigma *= term;
            sf *= ((seq_t)p + 1);
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + 1);
        sf *= ((seq_t)m + 1);
    }

    seq_t odd_sum = sigma - sf;
    return (seq_t)(sf & odd_sum);
}
