/*
 * A344224  a(n) = Sum_{k=1..n} tau(gcd(k,n)^gcd(k,n)), where tau(n) is the
 *          number of divisors of n.
 * https://oeis.org/A344224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(gcd(k,n)^gcd(k,n))
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 329, u64 n <= 6929,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum_{d|n} phi(n/d) * tau(d^d).  Where tau(m) is the number of divisors of m. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"

static void gen_divs(size_t idx, uint64_t n, uint64_t d,
                     uint64_t *pr, uint64_t *ex,
                     size_t m, int *curr, seq_t *sum)
{
    if (idx == m) {
        /* leaf: d is complete divisor */
        seq_t d_seq = (seq_t)d;
        seq_t phi = calc_phi((seq_t)(n / d));

        seq_t tau = 1;
        for (size_t i = 0; i < m; ++i) {
            int f = curr[i];
            if (f > 0) {
                seq_t term = ((seq_t)f) * d_seq + 1;
                tau = tau * term;
            }
        }

        *sum = *sum + phi * tau;
        return;
    }

    uint64_t prime = pr[idx];
    uint64_t pow = 1;                     /* prime^exp  (exp=0 at start) */
    for (uint64_t exp = 0; exp <= ex[idx]; ++exp) {
        curr[idx] = (int)exp;
        gen_divs(idx + 1, n, d * pow, pr, ex, m, curr, sum);
        pow *= prime;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0, but guard anyway */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    int curr_exp[64] = {0};

    seq_t result = 0;
    gen_divs(0, (uint64_t)n, 1, primes, exps, pcnt, curr_exp, &result);
    return result;
}
