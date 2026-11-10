/*
 * A349632  Dirichlet convolution of A250469 with A346234, which is Dirichlet
 *          inverse of A003961.
 * https://oeis.org/A349632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A250469(d) * A346234(n/d), where A346234(d) = 0
 *            unless d is squarefree, and then (-1)^omega(d) * Product_{p|d}
 *            nextprime(p). A250469(1) = 1; for n > 1 with p the smallest
 *            prime factor of n, A250469(n) moves n from row p of the sieve
 *            table A083221 to row nextprime(p), keeping its...
 * Range:     i8 n <= 65, i16 n <= 3989, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9470 (2026-11-10)
 * Helpers:   factor_u64, next_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/smallest_prime_factor.h"

/* true if j has no divisor t with 2 <= t < p */
static bool A349632_rough(uint64_t j, uint64_t p)
{
    for (uint64_t t = 2; t < p && t * t <= j; ++t)
        if (j % t == 0) return false;
    if (j > 1 && j < p) return false; /* j itself is a prime below p */
    return true;
}

static seq_t A250469_a(uint64_t d)
{
    if (d == 1) return (seq_t)1;
    uint64_t p = smallest_prime_factor(d);
    uint64_t m = d / p;
    if (p == 2) return (seq_t)3 * ((seq_t)2 * (seq_t)m - 1);
    /* rank of m among numbers (including 1) with no prime factor < p */
    uint64_t rank = 0;
    for (uint64_t j = 1; j <= m; ++j)
        if (A349632_rough(j, p)) ++rank;
    uint64_t q = next_prime(p);
    uint64_t j = 0, c = 0;
    while (c < rank) {
        ++j;
        if (A349632_rough(j, q)) ++c;
    }
    return (seq_t)q * (seq_t)j;
}

seq_t a(idx_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t nn = (uint64_t)n;
    if (nn == 1) return (seq_t)1;
    factor_u64(nn, pr, ex, &cnt);
    seq_t total = 0;
    uint64_t lim = (uint64_t)1 << cnt;
    for (uint64_t mask = 0; mask < lim; ++mask) {
        uint64_t m = 1;
        seq_t f = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                m *= pr[i];
                f *= -(seq_t)next_prime(pr[i]);
            }
        }
        total += f * A250469_a(nn / m);
    }
    return total;
}
