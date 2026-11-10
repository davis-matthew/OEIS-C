/*
 * A258997  A(n,k) = pi-based antiderivative of n^k; square array A(n,k),
 *          n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A258997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = k*n^(k-1)*A258851(n), A(n,0)=0; array read by
 *            antidiagonals (n ascending, k = d-n)
 * Range:     u8 n <= 38, u16 n <= 69, u32 n <= 158, u64 n <= 413,
 *            u128 n <= 1137, big any n
 * Validated: standard, pass, 82 terms through n = 9475 (2026-11-07)
 * Helpers:   prime_pi, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"
#include "../utils/smallest_prime_factor.h"

static seq_t A258851_a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;
    idx_t m = n;
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
        }
        seq_t pi_val = prime_pi((idx_t)p);
        result += (seq_t)e * pi_val * (seq_t)(n / p);
    }
    return result;
}

seq_t a(idx_t idx) {
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= idx) d++;
    idx_t n = idx - d * (d + 1) / 2;
    idx_t k = d - n;
    if (k == 0 || n <= 1) return 0;
    seq_t r = A258851_a(n);
    r *= (seq_t)k;
    for (idx_t i = 1; i < k; i++) r *= (seq_t)n;
    return r;
}
