/*
 * A341593  Number of superior prime-power divisors of n.
 * https://oeis.org/A341593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of superior prime‑power divisors of n
 *  A divisor d of n is superior if d ≥ n/d, i.e. d² ≥ n.
 *  We count those superior divisors that are prime powers.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, since 1 has no prime‑power divisors */
    if (n <= 1)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    uint64_t nn = (uint64_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t prod = 1;                    /* p^k for k from 1..e */
        for (uint64_t k = 1; k <= e; ++k) {
            prod *= p;                        /* now prod = p^k */

            /* prod is superior iff prod² ≥ nn.
               Avoid 128‑bit arithmetic by using
               prod >= ceil(nn/prod) == prod >= ((nn-1)/prod + 1).   */
            uint64_t ceil_div = (nn - 1) / prod + 1;
            if (prod >= ceil_div)
                ++result;
        }
    }

    return result;
}
