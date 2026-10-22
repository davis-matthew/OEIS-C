/*
 * A348504  a(n) = sigma(n) / gcd(sigma(n), usigma(n)), where sigma is the sum
 *          of divisors function, A000203, and usigma is the unitary sigma,
 *          A034448.
 * https://oeis.org/A348504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), usigma(n))
 * Range:     u8 n <= 22, u16 n <= 812, u32 n <= 9752, u64 n <= 9752,
 *            u128 n <= 9752, big any n
 * Helpers:   seq_pow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow_local.h"

static seq_t gcd_seq_local(seq_t a, seq_t b) {
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    idx_t temp = n;
    seq_t sigma  = 1;
    seq_t usigma = 1;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t p = 2;
        seq_t sigma_part = (seq_pow_local(p, e + 1) - 1) / (p - 1);
        sigma *= sigma_part;
        usigma *= 1 + seq_pow_local(p, e);
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pseq = (seq_t)p;
            seq_t sigma_part = (seq_pow_local(pseq, e + 1) - 1) / (pseq - 1);
            sigma *= sigma_part;
            usigma *= 1 + seq_pow_local(pseq, e);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        seq_t sigma_part = (seq_pow_local(pseq, 2) - 1) / (pseq - 1);
        sigma *= sigma_part;
        usigma *= 1 + pseq;
    }

    seq_t g = gcd_seq_local(sigma, usigma);
    return sigma / g;
}
