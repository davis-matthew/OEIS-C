/*
 * A348977  a(n) = gcd(sigma(n), A332993(n)).
 * https://oeis.org/A348977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), A332993(n)), where A332993(n) = n +
 *            A332993(n/spf(n)), A332993(1) = 1
 * Range:     u8 n <= 241, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 9295 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A348977_sigma_and_chain(idx_t n, seq_t *chain_out)
{
    seq_t sigma = 1;
    seq_t chain = 0;
    uint64_t m = (uint64_t)n;
    uint64_t cur = (uint64_t)n;

    /* factor n by trial division, computing sigma */
    uint64_t x = m;
    uint64_t p;
    for (p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (x % p == 0) {
                x /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            sigma *= term;
        }
    }
    if (x > 1) sigma *= ((seq_t)x + 1);

    /* chain of largest proper divisors: divide by smallest prime factor */
    while (cur > 1) {
        chain += (seq_t)cur;
        uint64_t spf = cur;
        if (cur % 2 == 0) spf = 2;
        else {
            for (uint64_t q = 3; q * q <= cur; q += 2) {
                if (cur % q == 0) { spf = q; break; }
            }
        }
        cur /= spf;
    }
    chain += 1;

    *chain_out = chain;
    return sigma;
}

seq_t a(idx_t n)
{
    seq_t chain;
    seq_t sigma = A348977_sigma_and_chain(n, &chain);
    return gcd(sigma, chain);
}
