/*
 * A372939  a(n) = Sum_{1 <= x_1, x_2, ... , x_n <= n} gcd(x_1, x_2, ... ,
 *          x_n, n)^n.
 * https://oeis.org/A372939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * d^n * tau(d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   factor_u64, ipow, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n) {
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* generate all divisors of n */
    seq_t divs[512];
    size_t cur_len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t prev_len = cur_len;
        for (size_t j = 0; j < prev_len; ++j) {
            seq_t base = divs[j];
            seq_t mult = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                mult *= (seq_t)p;
                divs[cur_len++] = base * mult;
            }
        }
    }

    /* accumulate contributions */
    seq_t pos = 0;
    seq_t neg = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 0; i < cur_len; ++i) {
        seq_t d = divs[i];
        idx_t d_idx = (idx_t)d;
        int mu = mu_of(n / d_idx);
        if (mu == 0) continue;
        seq_t d_pow = ipow(d, n_seq);
        seq_t tau_d = (seq_t)num_divisors((uint64_t)d_idx);
        seq_t term = d_pow * tau_d;
        if (mu == 1) {
            pos += term;
        } else { /* mu == -1 */
            neg += term;
        }
    }

    seq_t result = pos;
    result -= neg;
    return result;
}
