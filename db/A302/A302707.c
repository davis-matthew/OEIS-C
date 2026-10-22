/*
 * A302707  Number of factors of Chebyshev polynomial S(2*n+1, x) (A049310)
 *          over the integers. Factorization is into the minimal integer
 *          polynomials C (A187360).
 * https://oeis.org/A302707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau_odd(n+1) + tau(2*(n+1)) - 2 where tau_odd(m) is the
 *            number of odd divisors of m
 * Range:     u8 n <= 9274, u16 n <= 9274, u32 n <= 9274, u64 n <= 9274,
 *            u128 n <= 9274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Count the number of positive divisors of a 64‑bit integer x. */
static seq_t divisor_count_u64(uint64_t x) {
    if (x <= 1) return (seq_t)1;          /* tau(0) and tau(1) = 1 */
    uint64_t res = 1;
    uint64_t cnt, p;

    /* factor 2 */
    cnt = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        cnt++;
    }
    if (cnt > 0) res *= (cnt + 1);

    /* odd factors */
    for (p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            res *= (cnt + 1);
        }
    }
    if (x > 1) res *= 2;          /* remaining prime factor */

    return (seq_t)res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* we work with unsigned 64 bits; n is guaranteed to fit in idx_t (uint64_t) */
    uint64_t m = (uint64_t)n + 1ULL;          /* m = n+1 */
    /* odd part of m for tau_odd(m) */
    uint64_t odd = m;
    while ((odd & 1ULL) == 0) odd >>= 1;
    seq_t tau_odd_m  = divisor_count_u64(odd);
    seq_t tau_2m     = divisor_count_u64(2ULL * m);

    return (seq_t)(tau_odd_m + tau_2m - 2ULL);
}
