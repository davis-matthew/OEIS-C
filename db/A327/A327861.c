/*
 * A327861  Number of divisors d of n for which A003415(d)*d is equal to n,
 *          where A003415(x) gives the arithmetic derivative of x.
 * https://oeis.org/A327861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d|n : d * A003415(d) = n } where A003415 is the
 *            arithmetic derivative
 * Range:     u8 n <= 640, u16 n <= 9867, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate all divisors of n and count those satisfying
   d * A003415(d) == n. */
static void enumerate_divisors(size_t idx, seq_t curr_d,
                               uint64_t *primes, uint64_t *exps, size_t k,
                               uint64_t *div_exp, seq_t target, seq_t *count)
{
    if (idx == k) {
        /* compute arithmetic derivative of the divisor d=curr_d */
        seq_t sum = 0;
        for (size_t j = 0; j < k; ++j) {
            uint64_t a = div_exp[j];
            if (a == 0) continue;
            seq_t d_div = curr_d / primes[j];
            seq_t term  = (seq_t)a * d_div;
            sum += term;
        }
        seq_t product = curr_d * sum;
        if (product == target)
            (*count)++;
        return;
    }

    seq_t pow = 1;                     /* primes[idx]^a */
    for (uint64_t a = 0; a <= exps[idx]; ++a) {
        div_exp[idx] = a;
        enumerate_divisors(idx + 1, curr_d * pow,
                           primes, exps, k,
                           div_exp, target, count);
        pow *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* below the offset */

    uint64_t un = (uint64_t)n;
    seq_t target = (seq_t)un;

    /* Factor n into primes and exponents */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t  cnt = 0;
    factor_u64(un, primes, exps, &cnt);

    /* Enumerate divisors and count */
    uint64_t div_exp[20];
    seq_t    count = 0;
    enumerate_divisors(0, (seq_t)1, primes, exps, cnt,
                       div_exp, target, &count);
    return count;
}
