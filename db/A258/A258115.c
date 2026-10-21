/*
 * A258115  a(n) = A208570(n)/n.
 * https://oeis.org/A258115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime p that minimizes p^(1+nu_p(n)) over all primes,
 *            where nu_p(n) is the p-adic order of n. The function returns
 *            a(idx_t n) and uses helper routines from the utils directory
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the smallest prime not dividing n.  This will be the
       candidate with nu_p(n) = 0, so its cost is simply p.  */
    idx_t p = 2;
    while (true) {
        if (is_prime(p) && (n % p != 0))
            break;
        ++p;
    }

    seq_t min_cost = (seq_t)p;   /* cost of the current best prime */
    idx_t best = p;              /* best prime so far */

    /* Factor n and examine each prime divisor.  For a divisor p with
       exponent e, its cost is p^(1+e).  If this cost is less than the
       current minimum (which is either p0 or a smaller divisor), update
       the best prime.  */
    uint64_t primes[64];   /* array big enough for any 64‑bit n */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t cost = pow_uint(primes[i], exps[i] + 1);
        if (cost < min_cost) {
            min_cost = cost;
            best = primes[i];
        }
    }

    return (seq_t)best;
}
