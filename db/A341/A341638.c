/*
 * A341638  a(n) = Sum_{d|n} phi(d) * sigma(d) * tau(n/d).
 * https://oeis.org/A341638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} φ(d) * σ(d) * τ(n/d)
 * Range:     u8 n <= 14, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  φ  – Euler's totient (provided by ../utils/phi.h) */

#include <stdint.h>
#include "../utils/phi.h"

/* Sum of divisors of m */
static seq_t sum_of_divisors(idx_t m)
{
    seq_t s = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            seq_t i_s = (seq_t)i;
            idx_t j = m / i;
            if (i == j)
                s += i_s;
            else
                s += i_s + (seq_t)j;
        }
    }
    return s;
}

/* Count of divisors of m */
static seq_t divisor_count(idx_t m)
{
    seq_t c = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            if (i == m / i)
                ++c;
            else
                c += 2;
        }
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    seq_t total = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        seq_t phi_d   = phi(d);
        seq_t sigma_d = sum_of_divisors(d);
        idx_t m       = n / d;
        seq_t tau_m   = divisor_count(m);

        total += phi_d * sigma_d * tau_m;

        if (d != m) {              /* complementary divisor */
            seq_t phi_m   = phi(m);
            seq_t sigma_m = sum_of_divisors(m);
            seq_t tau_d   = divisor_count(d);
            total += phi_m * sigma_m * tau_d;
        }
    }
    return total;
}
