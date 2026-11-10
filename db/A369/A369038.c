/*
 * A369038  Numerator of ratio A003415(n) / A003415(A276086(n)), where A003415
 *          is the arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A369038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = A003415(n) / gcd(A003415(n), A003415(A276086(n))), with
 *            a(1) = 0
 * Range:     u8 n <= 233, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 9451 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A369038_deriv(uint64_t n)
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
    static const unsigned pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    if (n <= 1) return 0;
    uint64_t nn = (uint64_t)n;

    seq_t Dn = A369038_deriv(nn);

    /* primorial base digits */
    unsigned dig[16];
    int len = 0;
    uint64_t P = 1;
    for (int i = 0; i < 16; ++i) {
        if (P > nn) break;
        dig[i] = (unsigned)((nn / P) % pr[i]);
        len = i + 1;
        if (i == 15) break;
        if (P > UINT64_MAX / pr[i]) break;
        P *= pr[i];
    }

    /* p_i^e_i mod Dn */
    seq_t pw[16];
    for (int i = 0; i < len; ++i) {
        seq_t r = (seq_t)1 % Dn;
        for (unsigned k = 0; k < dig[i]; ++k) r = (r * (seq_t)pr[i]) % Dn;
        pw[i] = r;
    }

    seq_t D2 = 0;
    for (int i = 0; i < len; ++i) {
        if (dig[i] == 0) continue;
        seq_t acc = (seq_t)dig[i] % Dn;
        for (int j = 0; j < len; ++j) {
            if (j == i) {
                seq_t r = (seq_t)1 % Dn;
                for (unsigned k = 0; k + 1 < dig[i]; ++k) r = (r * (seq_t)pr[i]) % Dn;
                acc = (acc * r) % Dn;
            } else {
                acc = (acc * pw[j]) % Dn;
            }
        }
        D2 = (D2 + acc) % Dn;
    }

    seq_t g = gcd(Dn, D2);
    return Dn / g;
}
