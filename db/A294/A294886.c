/*
 * A294886  Sum of deficient proper divisors of n.
 * https://oeis.org/A294886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d<n, deficient(d)} d
 * Range:     u8 n <= 219, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* deficient(d) ⇔ sigma(d) < 2*d, where sigma is the sum of all divisors of d */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

/* local helper: compute the sum of all divisors of m */
static seq_t sigma_of(uint64_t m) {
    if (m == 1) return 1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        /* term = (p^(e+1) - 1) / (p - 1) */
        seq_t numerator = pow_seq(p, (int)(exps[i] + 1)) - 1;
        seq_t denom     = p - 1;
        seq_t term      = numerator / denom;
        result *= term;
    }
    return result;
}

/* recursively enumerate all divisors of n given its prime factorisation
   and add the deficient ones to *sum */
static void enumerate_divisors(const uint64_t *pr, const uint64_t *ex,
                               size_t cnt, uint64_t current,
                               uint64_t n, seq_t *sum) {
    if (cnt == 0) {
        if (current == n) return;            /* skip n itself */
        seq_t sigma = sigma_of(current);
        /* sigma < 2*current  ⇔  deficient */
        seq_t twice  = (seq_t)current * 2;
        if (sigma < twice) *sum += (seq_t)current;
        return;
    }
    uint64_t p = pr[0];
    uint64_t e = ex[0];
    uint64_t val = current;
    for (uint64_t k = 0; k <= e; ++k) {
        enumerate_divisors(pr + 1, ex + 1, cnt - 1, val, n, sum);
        val *= p;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                  /* undefined for n = 0, return 0 */
    uint64_t num = (uint64_t)n;
    if (num == 1) return 0;                /* no proper divisors */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(num, primes, exps, &cnt);

    seq_t sum = 0;
    enumerate_divisors(primes, exps, cnt, 1, num, &sum);
    return sum;
}
