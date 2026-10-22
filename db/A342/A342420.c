/*
 * A342420  a(n) = Sum_{k=1..n} (n/gcd(k,n))^(n/gcd(k,n)).
 * https://oeis.org/A342420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * d^d (OEIS A342420)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(d) for d ≤ 10000 using trial division. */
static seq_t phi_u64(uint64_t d)
{
    seq_t result = (seq_t)d;
    uint64_t n = d;

    if (n % 2 == 0) {
        result -= result / 2;
        while (n % 2 == 0) n /= 2;
    }

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            result -= result / p;
            while (n % p == 0) n /= p;
        }
    }

    if (n > 1) {
        result -= result / n;
    }

    return result;
}

/* Compute base^exp, where exp fits in idx_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, return 0 */

    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            const uint64_t d1 = (uint64_t)i;
            seq_t phi1 = phi_u64(d1);
            seq_t pow1 = pow_seq((seq_t)d1, d1);
            sum += phi1 * pow1;

            /* corresponding divisor n/i if distinct */
            if (i != (n / i)) {
                const uint64_t d2 = n / i;
                seq_t phi2 = phi_u64(d2);
                seq_t pow2 = pow_seq((seq_t)d2, d2);
                sum += phi2 * pow2;
            }
        }
    }

    return sum;
}
