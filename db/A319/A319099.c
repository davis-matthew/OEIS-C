/*
 * A319099  Number of solutions to x^5 == 1 (mod n).
 * https://oeis.org/A319099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of solutions to x^5 == 1 (mod n) For n > 0, a is
 *            the multiplicative function defined by: a(5) = 1, a(5^e) = 5 for
 *            e ≥ 2 For other primes p: a(p^e) = 5 if p ≡ 1 (mod 5), 1
 *            otherwise a(n) = ∏ a(p^e) over the prime‑power factors of n. See
 *            OEIS A319099
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 1, and a(0) is undefined */
        return 1;

    /* Factor n into primes and exponents.
       The helper stores primes in primes[], exps[].
       The count is returned in "cnt". */
    uint64_t primes[128];
    uint64_t exps  [128];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;
        if (p == 5ULL) {
            /* a(5) = 1, a(5^e) = 5 for e ≥ 2 */
            val = (e == 1) ? 1 : 5;
        } else {
            /* For primes other than 5, exponent doesn’t matter. */
            val = (p % 5ULL == 1ULL) ? 5 : 1;
        }
        result *= val;
    }
    return result;
}
