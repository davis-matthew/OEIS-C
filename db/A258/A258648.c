/*
 * A258648  Eighth arithmetic derivative of n.
 * https://oeis.org/A258648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A003415^8(n)  (the eighth arithmetic derivative of n)
 * A003415: arithmetic derivative defined by d(p) = 1 for prime p and
 * d(ab) = d(a)b + a d(b).
 *
 * The implementation applies the derivative formula using prime
 * factorization n = ∏ p_i^e_i → d(n) = n * Σ (e_i / p_i).
 * It repeats this operation eight times.  For n ≤ 10000 all
 * intermediate values fit in uint64_t, so we use unsigned 64‑bit
 * arithmetic for factorization and accumulation and cast the final
 * result to seq_t.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    for (int iter = 0; iter < 8; ++iter)
    {
        if (m == 0 || m == 1)
        {
            m = 0;
            break;
        }

        /* factor m */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* compute d(m) = Σ e_i * m / p_i */
        uint64_t sum = 0;
        for (size_t i = 0; i < cnt; ++i)
        {
            /* m is divisible by primes[i] */
            uint64_t term = (m / primes[i]) * exps[i];
            sum += term;
        }
        m = sum;
    }

    return (seq_t)m;
}
