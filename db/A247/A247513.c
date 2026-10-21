/*
 * A247513  Number of elements in the set {(x,y,z): 1<=x,y,z<=n, gcd(x,y,z)=1,
 *          lcm(x,y,z)=n}.
 * https://oeis.org/A247513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} (6·e) (equivalently, 6^ω(n) * ∏ e_i)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t np = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(np, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned m = (unsigned)(exps[i] * 6U);
        result = mul_small(result, m);
    }

    return result;
}
