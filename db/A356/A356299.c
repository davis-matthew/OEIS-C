/*
 * A356299  a(n) = gcd(A276086(n), A342001(n)), where A276086 is the primorial
 *          base exp-function, and A342001 is the arithmetic derivative
 *          without its inherited divisor.
 * https://oeis.org/A356299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A276086(n), A342001(n)); computed per prime as the
 *            product of p_{k+1}^min(d_k, v_p(A342001(n))), where d_k are the
 *            primorial-base digits of n
 * Range:     u8 n <= 405, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8315 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool gcdp_is_prime(idx_t p)
{
    if (p < 2) return false;
    for (idx_t d = 2; d <= p / d; ++d)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* A342001(n) = sum v * rad/p */
    seq_t m = 0;
    if (n > 1) {
        idx_t fp[64];
        idx_t fe[64];
        int cnt = 0;
        idx_t t = n;
        for (idx_t p = 2; p <= t / p; ++p) {
            if (t % p == 0) {
                idx_t e = 0;
                while (t % p == 0) { t /= p; ++e; }
                fp[cnt] = p; fe[cnt] = e; ++cnt;
            }
        }
        if (t > 1) { fp[cnt] = t; fe[cnt] = 1; ++cnt; }
        seq_t rad = 1;
        for (int i = 0; i < cnt; ++i) rad *= (seq_t)fp[i];
        for (int i = 0; i < cnt; ++i)
            m += (seq_t)fe[i] * (rad / (seq_t)fp[i]);
    }

    seq_t result = 1;
    idx_t rem = n;
    idx_t p = 2;
    while (rem > 0) {
        idx_t d = rem % p;
        rem /= p;
        if (d > 0) {
            idx_t e = 0;
            if (m == 0) {
                e = d;
            } else {
                seq_t mm = m;
                while (e < d && mm % (seq_t)p == 0) { mm /= (seq_t)p; ++e; }
            }
            for (idx_t i = 0; i < e; ++i) result *= (seq_t)p;
        }
        do { ++p; } while (!gcdp_is_prime(p));
    }
    return result;
}
