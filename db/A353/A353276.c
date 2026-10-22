/*
 * A353276  a(n) = phi(n) + tau(n)^omega(n) - sigma(n).
 * https://oeis.org/A353276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = phi(n) + tau(n)^omega(n) - sigma(n)
 * Range:     i8 n <= 29, i16 n <= 209, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_pow_int(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t phi   = (seq_t)n;
    seq_t tau   = 1;
    seq_t sigma = 1;
    int   omega = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            ++omega;
            seq_t sp = (seq_t)p;
            phi = phi / sp * (sp - 1);
            tau = tau * (seq_t)(exp + 1);

            seq_t sum   = 1;
            seq_t p_pow = 1;
            for (int j = 0; j < exp; ++j) {
                p_pow *= sp;
                sum += p_pow;
            }
            sigma *= sum;
        }
    }

    if (temp > 1) {   // last prime factor
        uint64_t p = temp;
        seq_t sp = (seq_t)p;
        ++omega;
        phi = phi / sp * (sp - 1);
        tau = tau * 2;
        seq_t sum = 1 + sp;
        sigma *= sum;
    }

    seq_t pow_tau_omega = seq_pow_int(tau, omega);
    return phi + pow_tau_omega - sigma;
}
