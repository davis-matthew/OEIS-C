/*
 * A355618  a(n) is the least prime that is the sum of a list of numbers > 1
 *          whose product is n, or -1 if there is no such prime.
 * https://oeis.org/A355618
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = least prime that is the sum of a list of numbers > 1
 *            whose product is n, or -1 if no such prime exists
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static void dfs(uint64_t rem, uint64_t last, seq_t sum, seq_t *best)
{
    /* Consider taking rem as the last factor in the factorization. */
    seq_t candidate = sum + (seq_t)rem;
    if (is_prime_big(candidate)) {
        if (*best < 0 || candidate < *best) {
            *best = candidate;
        }
    }

    /* Try further splits with factors >= last. */
    for (uint64_t f = last; f <= rem / f; ++f) {
        if (rem % f == 0) {
            dfs(rem / f, f, sum + (seq_t)f, best);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)-1;          /* below offset */

    /* 1. If n itself is prime, it is the answer. */
    if (is_prime(n))
        return (seq_t)n;

    /* 2. Sum of prime factorization with multiplicity. */
    uint64_t primes[64];
    uint64_t exps[64];   /* must be uint64_t for factor_u64 */
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_pf = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum_pf += (seq_t)primes[i] * (seq_t)exps[i];

    if (is_prime_big(sum_pf))
        return sum_pf;

    /* 3. Search all factorizations for the smallest prime sum. */
    seq_t best = -1;
    dfs((uint64_t)n, 2, 0, &best);

    return best;
}
