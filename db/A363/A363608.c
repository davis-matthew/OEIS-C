/*
 * A363608  Expansion of Sum_{k>0} x^(4*k)/(1-x^k)^5.
 * https://oeis.org/A363608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(d,4)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute binom(d,4) safely in a 128‑bit intermediate. */
static inline seq_t binom4(idx_t d) {
    if (d < 4) return 0;
    __int128 t = (__int128)d * (d - 1) * (d - 2) * (d - 3);
    t /= 24;
    long long tmp = (long long)t;  /* d ≤ 10⁴ → t fits in 64‑bit */
    return (seq_t)tmp;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t sum = 0;
    /* floor(sqrt(n)) – double precision is exact for n ≤ 10⁴ */
    idx_t root = (idx_t) sqrt((double)n);
    while (root * root > n) root--;
    while ((root + 1) * (root + 1) <= n) root++;

    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            sum += binom4(i);
            idx_t j = n / i;
            if (j != i) sum += binom4(j);
        }
    }
    return sum;
}
