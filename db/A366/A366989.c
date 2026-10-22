/*
 * A366989  The number of prime powers p^q dividing n, where p is prime and q
 *          is either 1 or prime (A334393 without the first term 1).
 * https://oeis.org/A366989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e || n} (1 + π(e)), where π(e) counts primes ≤ e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the additive expression in the OEIS entry: */
/* a(p^e) = A000720(e) + 1, and a(n) is the sum over all prime powers dividing n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"  /* for factor_u64 */

seq_t a(idx_t n)
{
    /* Precomputed prime-count function π(e) for e up to 63 (max exponent in 64-bit). */
    static seq_t pi_table[65];
    static bool initialized = false;
    if (!initialized) {
        bool is_prime[65] = {0};
        for (int i = 2; i <= 64; ++i) is_prime[i] = true;
        for (int i = 2; i * i <= 64; ++i)
            if (is_prime[i])
                for (int j = i * i; j <= 64; j += i)
                    is_prime[j] = false;
        pi_table[0] = pi_table[1] = 0;
        for (int i = 2; i <= 64; ++i)
            pi_table[i] = pi_table[i - 1] + (is_prime[i] ? 1 : 0);
        initialized = true;
    }

    if (n == 1) return 0;

    /* Factor n: primes and exponents arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t contrib = 1 + pi_table[(int)e];
        result += contrib;
    }
    return result;
}
