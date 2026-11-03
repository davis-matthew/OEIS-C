/*
 * A339667  Number of nonempty subsets of divisors of n having a common factor
 *          > 1.
 * https://oeis.org/A339667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{d|n, d < n} mu(n/d) * (2^tau(d) - 1) where tau is
 *            the divisor‑count function and mu the Möbius function
 * Range:     u8 n <= 47, u16 n <= 239, u32 n <= 1679, u64 n <= 15119,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9803 (2026-11-01)
 * Helpers:   factor_u64, num_divisors, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/num_divisors.h"
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    if (n <= 1)  /* a(1) = 0, a(0) is undefined but return 0 safely */
        return (seq_t)0;

    /* Prime factorization of n: primes 'pr' and exponents 'ex'. */
    uint64_t pr[32], ex[32];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pf_cnt);

    /* Enumerate all divisors of n (including 1 and n) in a static array. */
    /* The maximum number of divisors for n <= 10000 is 128, so this size is safe. */
    idx_t divs[2048];
    size_t div_len = 1;
    divs[0] = 1;                     /* start with divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t exp = ex[i];
        size_t cur_len = div_len;

        idx_t pow_p = 1;
        for (uint64_t k = 1; k <= exp; ++k) {
            pow_p *= p;
            for (size_t j = 0; j < cur_len; ++j)
                divs[div_len++] = divs[j] * pow_p;
        }
    }

    seq_t pos_sum = 0; /* contributions from µ(n/d) = +1 (will be subtracted) */
    seq_t neg_sum = 0; /* contributions from µ(n/d) = -1 (will be added) */

    for (size_t i = 0; i < div_len; ++i) {
        idx_t d = divs[i];
        if (d == n)          /* exclude divisor equal to n */
            continue;

        int mu = mu_of((idx_t)(n / d));
        if (mu == 0)
            continue;        /* µ = 0 contributes nothing */

        /* t = 2^τ(d) - 1 */
        uint64_t tau_d = num_divisors((uint64_t)d);
        seq_t t = pow2((idx_t)tau_d) - (seq_t)1;

        if (mu == 1)
            pos_sum += t;     /* will be subtracted later */
        else            /* mu == -1 */
            neg_sum += t;
    }

    /* Result is neg_sum - pos_sum; it is guaranteed non‑negative. */
    if (neg_sum < pos_sum)
        return (seq_t)0;       /* safety fallback – should not occur */
    return neg_sum - pos_sum;
}
