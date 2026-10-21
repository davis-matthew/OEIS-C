/*
 * A248766  Greatest 4th-power-free divisor of n!
 * https://oeis.org/A248766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest 4th‑power‑free divisor of n!
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = ∏_{p≤n} p^{(∑_{k≥1} ⌊n / p^k⌋) mod 4}  */

#include <stdlib.h>
#include "../utils/build_primes.h"

/* Lazy static cache of primes up to the largest n requested. */
static idx_t  *cached_primes   = NULL;
static size_t  cached_cnt      = 0;
static idx_t   cached_limit    = 0;

/* Ensure primes up to limit are cached.  Returns the number of primes. */
static void ensure_primes(idx_t limit)
{
    if (limit <= cached_limit) return;
    if (cached_primes) free(cached_primes);
    build_primes(limit, &cached_primes, &cached_cnt);
    cached_limit = limit;
}

/* Greatest 4th‑power‑free divisor of n! */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;          /* offset starts at 1 */

    ensure_primes(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cached_cnt && cached_primes[i] <= n; ++i) {
        idx_t p = cached_primes[i];
        /* Compute exponent of p in n! modulo 4. */
        idx_t exp = 0, t = n;
        while (t) {
            t /= p;
            exp += t;
        }
        idx_t e_mod4 = exp & 3UL;
        if (e_mod4 == 0) continue;

        /* Multiply result by p^e_mod4 (e_mod4 ≤ 3). */
        seq_t mult = (seq_t)1;
        for (idx_t k = 0; k < e_mod4; ++k)
            mult *= (seq_t)p;
        result *= mult;
    }

    return result;
}
