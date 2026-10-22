/*
 * A352055  Sum of the 9th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 * Sum_{d|n, d<n, d odd} 1/d^9 Algorithm: - Compute
 *            the odd part of n (remove all factors 2). - Factor that odd part
 *            into primes. - Recursively generate all its divisors d (which
 *            are all odd divisors of n). - For each d < n, add (n/d)^9 to the
 *            sum (skip d == n when n is odd). - Return t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_seq.h"     /* pow_seq(seq_t a, int k) returns seq_t */

/* Helper to recursively build odd divisors of the odd-part of n. */
static void add_div_sum(idx_t n, idx_t m, const uint64_t *pr,
                        const uint64_t *ex, size_t pos, size_t cnt,
                        seq_t *sum, uint64_t cur_div)
{
    if (pos == cnt) {
        /* cur_div is a divisor d of the odd part.  Skip d = n when n is odd. */
        if (m == n && cur_div == (uint64_t)n)
            return;
        idx_t q = n / cur_div;                 /* q = n/d */
        seq_t val = pow_seq((seq_t)q, 9);       /* (n/d)^9 */
        *sum += val;
        return;
    }

    uint64_t p = pr[pos];
    uint64_t e = ex[pos];
    uint64_t v = cur_div;
    for (uint64_t exp = 0; exp <= e; ++exp) {
        add_div_sum(n, m, pr, ex, pos + 1, cnt, sum, v);
        v *= p;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Remove all factors of 2: odd part of n */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    /* Factor the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Generate all odd divisors and accumulate the sum */
    add_div_sum(n, m, primes, exps, 0, cnt, &sum, 1);

    return sum;
}
