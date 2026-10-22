/*
 * A338662  a(n) = Sum_{d|n} (n/d)^d * binomial(d+n/d-1, d).
 * https://oeis.org/A338662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^d * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 47, u64 n <= 104, u128 n <= 221,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

/* Helper: recursively generate all divisors of n and accumulate the sum. */
static void dfs_divisors(uint64_t n, const uint64_t *primes, const uint64_t *exps,
                         size_t num_p, size_t pos, uint64_t cur, seq_t *sum)
{
    if (pos == num_p) {
        /* cur is a divisor d of n */
        uint64_t d = cur;
        uint64_t m = n / d;                    /* n/d */
        seq_t pow_term = big_pow((seq_t)m, (seq_t)d);
        seq_t bin_term = binomial(d + m - 1, d);
        *sum += pow_term * bin_term;
        return;
    }

    uint64_t p   = primes[pos];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        dfs_divisors(n, primes, exps, num_p, pos + 1, cur * val, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used, but safe */

    /* Factor n into primes & exponents (max 64 prime factors for uint64). */
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t   num_p = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &num_p);

    seq_t sum = 0;
    dfs_divisors((uint64_t)n, primes_arr, exps_arr, num_p, 0, 1ULL, &sum);
    return sum;
}
