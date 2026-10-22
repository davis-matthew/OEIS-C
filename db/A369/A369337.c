/*
 * A369337  Total number of numbers less than each divisor d of n that do not
 *          divide d.
 * https://oeis.org/A369337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ₁(n) – ∏_{p^e‖n} ((e+1)(e+2)/2)
 * Range:     u8 n <= 16, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  = Sum_{d|n} (d – τ(d))  (τ(d) = number of divisors of d) */
#include <stdint.h>

/* seq_t and idx_t are supplied externally */

static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t temp = n;
    seq_t sigma = (seq_t)1;
    seq_t tau_prod = (seq_t)1;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        idx_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        seq_t p_pow = power_seq((seq_t)2, exp + 1);
        seq_t sigma_term = (p_pow - (seq_t)1) / ((seq_t)2 - (seq_t)1);
        sigma *= sigma_term;
        seq_t term = (seq_t)(exp + 1) * (seq_t)(exp + 2) / (seq_t)2;
        tau_prod *= term;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        seq_t p_pow = power_seq((seq_t)p, exp + 1);
        seq_t sigma_term = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= sigma_term;
        seq_t term = (seq_t)(exp + 1) * (seq_t)(exp + 2) / (seq_t)2;
        tau_prod *= term;
    }

    /* if a prime factor remains > sqrt(n) */
    if (temp > 1) {
        seq_t p_pow = (seq_t)temp * (seq_t)temp;  /* p^(1+1) since exp=1 */
        seq_t sigma_term = (p_pow - (seq_t)1) / ((seq_t)temp - (seq_t)1);
        sigma *= sigma_term;
        seq_t term = (seq_t)2 * (seq_t)3 / (seq_t)2; /* (1+1)(1+2)/2 = 3 */
        tau_prod *= term;
    }

    return sigma - tau_prod;
}
