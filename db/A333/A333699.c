/*
 * A333699  a(n) = Sum_{d|n} phi(n/d) * pi(d).
 * https://oeis.org/A333699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * pi(d)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* local prime test (trial division) */
static bool is_prime_local(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

/* prime counting function */
static seq_t pi_local(idx_t n) {
    seq_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime_local(i)) ++cnt;
    }
    return cnt;
}

/* Euler's totient function by trial division */
static seq_t phi_local(idx_t m) {
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) {
        result -= result / (seq_t)temp;
    }
    return result;
}

/* the required sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safeguard for off‑by‑one calls */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        /* term for divisor d1 */
        sum += phi_local(d2) * pi_local(d1);

        if (d1 != d2) {
            /* term for divisor d2 */
            sum += phi_local(d1) * pi_local(d2);
        }
    }
    return sum;
}
