/*
 * A327499  Quotient of n over the maximum divisor of n whose prime
 *          multiplicities are distinct.
 * https://oeis.org/A327499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / d, where d is the maximum divisor of n * whose prime
 *            multiplicities (exponents in its prime factorisation) * are all
 *            distinct. For each n ≥ 1 the function returns a(n)
 * Range:     u8 n <= 869, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition */

    /* We factor the 64‑bit integer n.  For n ≤ 10 000 at most
     * 20 distinct prime factors can occur. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sort the primes in descending order.  The exponents stay
     * paired with their primes by swapping both arrays. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (primes[j] > primes[i]) {
                uint64_t tmp = primes[i];
                primes[i] = primes[j];
                primes[j] = tmp;
                tmp = exps[i];
                exps[i] = exps[j];
                exps[j] = tmp;
            }
        }
    }

    /* Keep track of which exponents have already been used.
     * Since the maximum exponent for a 64‑bit integer is 64,
     * a table of size 65 is sufficient. */
    bool used[65] = { false };

    seq_t divisor = (seq_t)1;             /* product of selected prime powers */

    for (size_t i = 0; i < cnt; ++i) {
        /* Find the largest unused exponent ≤ exps[i] */
        int chosen = 0;
        for (int e = (int)exps[i]; e >= 1; --e) {
            if (!used[e]) { chosen = e; break; }
        }
        if (chosen == 0) continue;        /* this prime cannot contribute */

        used[chosen] = true;

        /* Compute primes[i] ^ chosen using only seq_t arithmetic. */
        seq_t power = (seq_t)primes[i];
        for (int k = 1; k < chosen; ++k) {
            power *= (seq_t)primes[i];
        }
        divisor *= power;
    }

    seq_t N = (seq_t)n;
    return N / divisor;                  /* exact division, divisor|n */
}
