/*
 * A369039  Denominator of ratio A003415(n) / A003415(A276086(n)), where
 *          A003415 is the arithmetic derivative and A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A369039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = A003415(A276086(n)) / gcd(A003415(n),
 *            A003415(A276086(n))); A003415(m) = Sum_{p|m} v_p(m)*m/p, and
 *            A276086(n) = Prod prime(k+1)^d_k for the primorial-base digits
 *            d_k of n
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 388, u64 n <= 2306,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 83 terms through n = 9441 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static const unsigned A369039_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static seq_t A369039_deriv_n(uint64_t n)
{
    if (n <= 1) return 0;
    seq_t res = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { ++e; tmp /= p; }
            res += (seq_t)e * ((seq_t)(n / p));
        }
    }
    if (tmp > 1) res += (seq_t)(n / tmp);
    return res;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t prim[16];
    unsigned dig[16];
    int len = 0;
    prim[0] = 1;
    for (int k = 0; k < 15; ++k) {
        if (prim[k] > nn) break;
        dig[k] = (unsigned)((nn / prim[k]) % A369039_pr[k]);
        len = k + 1;
        prim[k + 1] = prim[k] * A369039_pr[k];
    }

    seq_t m = 1;
    for (int k = 0; k < len; ++k)
        for (unsigned j = 0; j < dig[k]; ++j)
            m *= (seq_t)A369039_pr[k];

    seq_t dm = 0;
    for (int k = 0; k < len; ++k) {
        if (dig[k] == 0) continue;
        dm += (seq_t)dig[k] * (m / (seq_t)A369039_pr[k]);
    }

    seq_t dn = A369039_deriv_n(nn);
    seq_t g = gcd(dn, dm);
    if (g == 0) return 1;
    return dm / g;
}
