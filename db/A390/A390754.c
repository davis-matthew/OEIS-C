/*
 * A390754  The Euler totient of the smallest cube divisible by n.
 * https://oeis.org/A390754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = φ(∏_{p^e‖n} p^{k}), k = smallest multiple of 3 ≥ e
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = ∏_{p^e‖n} (p-1)·p^{k-1}  where k = e+((3-e%3)%3)  */

/* The framework provides these helpers; include them on demand. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* sequence entry */
seq_t a(idx_t n)
{
    /* n ≥ 1 */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t rem   = e % 3ULL;
        uint64_t delta = (3ULL - rem) % 3ULL;          /* 0, 1, or 2 */
        uint64_t k     = e + delta;                   /* exponent in the smallest cube */
        uint64_t exp   = k - 1;                       /* exponent for p in φ(p^k) */

        seq_t pow_val = pow_uint(p, (idx_t)exp);      /* p^exp in seq_t */
        seq_t mult    = (seq_t)(p - 1ULL);
        mult *= pow_val;                              /* (p-1)*p^exp */

        result *= mult;                               /* accumulate φ(p^k) */
    }

    return result;
}
