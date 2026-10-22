/*
 * A321259  a(n) = sigma_n(n) - n^n.
 * https://oeis.org/A321259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n, d<n} d^n (sum of the n-th powers of proper
 *            divisors of n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Helper: recursively enumerate all divisors of n and accumulate d^n,
   skipping d == n. */
static void
add_divisor_powers(idx_t n,
                   size_t idx,
                   size_t total,
                   uint64_t *primes,
                   uint64_t *exponents,
                   seq_t cur,
                   seq_t *sum)
{
    if (idx == total) {
        /* cur is a divisor.  Exclude the divisor n itself. */
        seq_t N = (seq_t)n;
        if (cur < N) {
            seq_t term = big_pow(cur, N);
            *sum += term;
        }
        return;
    }
    uint64_t p = primes[idx];
    seq_t product = cur;
    for (uint64_t e = 0; e <= exponents[idx]; ++e) {
        add_divisor_powers(n, idx + 1, total, primes, exponents,
                           product, sum);
        if (e == exponents[idx])
            break;
        product = product * (seq_t)p;
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;   /* a(1) = 0 */

    /* Factorization of n.  For n <= 10000 the number of distinct primes
       is at most 6 (<10).  Allocate small static arrays. */
    uint64_t primes[10];
    uint64_t exponents[10];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t sum = (seq_t)0;
    add_divisor_powers(n, 0, cnt, primes, exponents, (seq_t)1, &sum);
    return sum;
}
