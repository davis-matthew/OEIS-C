/*
 * A385055  a(n) = [n = 1 or A076479(n) = Möbius(A067029(n))], where [.] is
 *          the Iverson bracket.
 * https://oeis.org/A385055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n = 1 or A076479(n) = mu(A067029(n))]; A076479(n) =
 *            (-1)^omega(n), A067029(n) = exponent of least prime factor of n;
 *            computed by trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8860 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a385055_mu_small(unsigned e)
{
    int r = 1;
    if (e == 0) return 1;
    for (unsigned p = 2; p * p <= e; p++) {
        if (e % p == 0) {
            e /= p;
            if (e % p == 0) return 0;
            r = -r;
        }
    }
    if (e > 1) r = -r;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    uint64_t t = (uint64_t)n;
    int cnt = 0;
    unsigned first_exp = 0;
    if (t % 2 == 0) {
        cnt++;
        unsigned e = 0;
        while (t % 2 == 0) { t /= 2; e++; }
        first_exp = e;
    }
    for (uint64_t i = 3; i <= t / i; i += 2) {
        if (t % i == 0) {
            cnt++;
            unsigned e = 0;
            while (t % i == 0) { t /= i; e++; }
            if (first_exp == 0) first_exp = e;
        }
    }
    if (t > 1) {
        cnt++;
        if (first_exp == 0) first_exp = 1;
    }
    int rad_mu = (cnt & 1) ? -1 : 1;
    return (rad_mu == a385055_mu_small(first_exp)) ? (seq_t)1 : (seq_t)0;
}
