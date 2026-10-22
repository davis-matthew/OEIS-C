/*
 * A386591  Number of divisors of n that are not balanced numbers.
 * https://oeis.org/A386591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that phi(d) does NOT
 *            divide sigma(d)
 * Range:     u8 n <= 1659, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the helper functions:
 *   factor_u64  - factor n into primes and exponents
 *   phi         - Euler's totient of a number
 *   sigma_of    - sum of divisors of a number
 * The function enumerates all divisors of n by recursive back‑tracking
 * on the prime factorisation and counts those for which sigma(d) % phi(d) != 0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

static void gen_divisors_idx(size_t idx, idx_t curr,
                             const uint64_t *pr, const uint64_t *ex, size_t cnt,
                             seq_t *non_balanced)
{
    if (idx == cnt) {
        seq_t phi_val = phi(curr);
        seq_t sigma_val = sigma_of(curr);
        /* count if phi does NOT divide sigma */
        if (sigma_val % phi_val != 0)
            (*non_balanced)++;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    idx_t val = curr;
    for (size_t k = 0; k <= e; ++k) {
        gen_divisors_idx(idx + 1, val, pr, ex, cnt, non_balanced);
        val = val * (idx_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, harmless */

    /* Factor n into primes and exponents (max 64 distinct primes for a 64‑bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64(n, primes, exps, &prime_cnt);

    seq_t non_balanced = 0;
    gen_divisors_idx(0, (idx_t)1, primes, exps, prime_cnt, &non_balanced);
    return non_balanced;
}
