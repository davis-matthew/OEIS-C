/*
 * A387715  Number of divisors d of n for which A003959(d) > 2*d, where
 *          A003959 is multiplicative with a(p^e) = (p+1)^e.
 * https://oeis.org/A387715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A003959(d) > 2*d]
 * Range:     u8 n <= 47, u16 n <= 8979, u32 n <= 8979, u64 n <= 8979,
 *            u128 n <= 8979, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003959 is multiplicative with a(p^e) = (p+1)^e */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void rec(size_t idx, seq_t cur_d, seq_t cur_a,
                const uint64_t *pr, const uint64_t *ex,
                size_t pcnt, seq_t *cnt)
{
    if (idx == pcnt) {
        if (cur_a > (seq_t)2 * cur_d)
            (*cnt)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e_max = ex[idx];
    /* pre‑compute powers of p and (p+1) */
    seq_t d_pow = (seq_t)1;
    seq_t a_pow = (seq_t)1;
    for (uint64_t e = 0; e <= e_max; ++e) {
        if (e > 0) {
            d_pow *= (seq_t)p;
            a_pow *= (seq_t)(p + 1);
        }
        rec(idx + 1,
            cur_d * d_pow,
            cur_a * a_pow,
            pr, ex, pcnt, cnt);
    }
}

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t count = 0;
    rec(0, (seq_t)1, (seq_t)1, primes, exps, pcnt, &count);
    return count;
}
