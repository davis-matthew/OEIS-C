/*
 * A353235  Number of divisors of n whose arithmetic derivative is odd.
 * https://oeis.org/A353235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of divisors d of n whose arithmetic derivative d'
 *            is odd. * The function implements the following rules: * - if n
 *            = 2^k * m with k >= 1 and m odd: a(n) = a(m) + τ(m) * - if n is
 *            odd: * let n = ∏ p_i^e_i, τ(n) = ∏ (e_i+1) * if all e_i are
 *            even: a(n) = (τ(n)-1)/2 * else: a(n) = τ(n)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;            /* a(1) = 0 */

    /* Count powers of 2 */
    idx_t e2 = 0;
    idx_t t = n;
    while ((t & 1ull) == 0) {        /* while t is even */
        e2++;
        t >>= 1;
    }

    if (e2 > 0) {                    /* n is even */
        idx_t m = t;                 /* odd part */
        seq_t tau = 1;               /* τ(m) */
        idx_t temp = m;

        for (idx_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                idx_t cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                tau *= (seq_t)(cnt + 1);
            }
        }
        if (temp > 1)                /* remaining prime factor */
            tau *= 2;

        return a(m) + tau;           /* a(n) = a(m) + τ(m) */
    }

    /* n is odd */
    seq_t tau = 1;                   /* τ(n) */
    bool all_even = true;
    idx_t temp = t;                  /* n itself (odd) */

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                cnt++;
            }
            tau *= (seq_t)(cnt + 1);
            if (cnt & 1u) all_even = false;
        }
    }
    if (temp > 1) {                  /* remaining prime factor (exponent 1) */
        tau *= 2;
        all_even = false;            /* exponent 1 is odd */
    }

    if (all_even)
        return (tau - 1) / 2;        /* (τ-1)/2 when all exponents even */
    else
        return tau / 2;              /* τ/2 otherwise */
}
