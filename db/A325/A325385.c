/*
 * A325385  a(n) = gcd(n-A048250(n), n-A162296(n)).
 * https://oeis.org/A325385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - A048250(n), n - A162296(n)), with A048250(n) =
 *            prod_{p|n}(p+1), A162296(n) = sigma(n) - A048250(n); gcd taken
 *            of absolute values
 * Range:     u8 n <= 287, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9028 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A325385_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A325385_absdiff(seq_t a, seq_t b)
{
    return a >= b ? a - b : b - a;
}

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

    seq_t nn = (seq_t)n;
    seq_t sq = sigma - sf; /* A162296(n) */
    seq_t x = A325385_absdiff(nn, sf);
    seq_t y = A325385_absdiff(nn, sq);
    return A325385_gcd(x, y);
}
