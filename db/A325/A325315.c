/*
 * A325315  Bitwise-XOR of absolute values of (n - A048250(n)) and (n -
 *          A162296(n)).
 * https://oeis.org/A325315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003987(|n - A048250(n)|, |n - A162296(n)|), i.e. bitwise
 *            XOR of the two absolute differences; A048250(n) =
 *            prod_{p|n}(p+1), A162296(n) = sigma(n) - A048250(n)
 * Range:     u8 n <= 179, u16 n <= 25199, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 104 terms through n = 9762 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t absdiff_A325315(seq_t x, seq_t y)
{
    return x >= y ? x - y : y - x;
}

seq_t a(idx_t n)
{
    seq_t sigma = 1;
    seq_t sf = 1;
    idx_t t = n;

    for (idx_t p = 2; p <= t / p; ++p) {
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

    seq_t nn = (seq_t)n;
    seq_t d1 = absdiff_A325315(nn, sf);
    seq_t d2 = absdiff_A325315(nn, sigma - sf);
    return d1 ^ d2;
}
