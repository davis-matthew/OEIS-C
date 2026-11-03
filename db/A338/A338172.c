/*
 * A338172  a(n) is the product of those divisors d of n such that tau(d)
 *          divides sigma(d).
 * https://oeis.org/A338172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   factor_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for n, multiply all divisors d such that tau(d) divides sigma(d). */
/* Uses recursive enumeration of divisors and the helper routines
   factor_u64, divisor_count_u64 and sigma_of. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static void gen_divisors(size_t idx,
                         uint64_t cur,
                         const uint64_t *pr,
                         const uint64_t *ex,
                         size_t sz,
                         seq_t *prod)
{
    if (idx == sz) {
        uint64_t d = cur;
        seq_t sigma = sigma_of((idx_t)d);
        seq_t tau   = (seq_t)num_divisors(d);
        if ((sigma % tau) == 0) {
            *prod = (*prod) * (seq_t)d;
        }
        return;
    }
    uint64_t p = pr[idx];
    uint64_t pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(idx + 1, cur * pow, pr, ex, sz, prod);
        pow *= p;
    }
}

seq_t a(idx_t n)
{
    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t product = 1;
    gen_divisors(0, 1, primes, exps, cnt, &product);
    return product;
}
