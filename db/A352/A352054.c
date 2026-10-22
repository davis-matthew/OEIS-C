/*
 * A352054  Sum of the 8th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * Σ_{d|n, d<n, d odd} 1/d^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors of the odd part of n and accumulate
   the sum of (n/d)^8, skipping the divisor equal to n when n is odd. */
static void enumerate_divisors(idx_t n_orig,
                               const uint64_t *pr, const uint64_t *ex,
                               size_t num, size_t pos,
                               idx_t cur_div, seq_t *sum,
                               bool skip_odd_n)
{
    if (pos == num) {
        idx_t d = cur_div;
        if (skip_odd_n && d == n_orig)
            return;                       /* skip d == n when n is odd */
        seq_t nd = (seq_t)n_orig / (seq_t)d;
        *sum += big_pow(nd, 8);           /* (n/d)^8  */
        return;
    }

    uint64_t p = pr[pos];
    uint64_t e = ex[pos];
    idx_t prod = cur_div;
    for (uint64_t i = 0; i <= e; ++i) {
        enumerate_divisors(n_orig, pr, ex, num, pos + 1, prod, sum, skip_odd_n);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                          /* a(1) = 0 */

    /* Extract the odd part of n */
    idx_t n_odd = n;
    while ((n_odd & 1) == 0)
        n_odd >>= 1;

    /* Factor the odd part */
    const size_t MAX_FACTORS = 16;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t nf = 0;
    if (n_odd > 1)
        factor_u64(n_odd, primes, exps, &nf);

    seq_t sum = 0;
    bool skip_odd_n = (n & 1) != 0;        /* n is odd? */

    if (nf == 0) {
        /* odd part is 1 -> only divisor is 1 */
        seq_t nd = (seq_t)n;
        sum = big_pow(nd, 8);
    } else {
        enumerate_divisors(n, primes, exps, nf, 0, 1, &sum, skip_odd_n);
    }

    return sum;
}
