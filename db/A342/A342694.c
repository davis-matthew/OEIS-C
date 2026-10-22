/*
 * A342694  a(n) = Sum_{d|n} d * mu(d) * floor(n/d^2).
 * https://oeis.org/A342694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * mu(d) * floor(n/d^2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* computes μ(d) : -1,0,+1  (1 is count of prime factors modulo 2) */
static int mu_of_uint64(idx_t v) {
    if (v == 1)
        return 1;
    int cnt = 0;
    for (idx_t p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            int e = 0;
            do {
                v /= p;
                ++e;
            } while (v % p == 0);
            if (e > 1)
                return 0;         /* square factor */
            ++cnt;
        }
    }
    if (v > 1)
        ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* not in the defined range, just a safe guard */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {    /* loop over divisors up to sqrt(n) */
        if (n % d != 0)
            continue;
        idx_t d2 = n / d;
        seq_t t  = n / (d * d);
        int mu_d = mu_of_uint64(d);
        if (mu_d != 0) {
            sum += (seq_t)d * (seq_t)mu_d * t;
        }
        if (d2 != d) {
            seq_t t2 = n / (d2 * d2);
            int mu_d2 = mu_of_uint64(d2);
            if (mu_d2 != 0) {
                sum += (seq_t)d2 * (seq_t)mu_d2 * t2;
            }
        }
    }
    return sum;
}
