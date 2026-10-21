/*
 * A254317  a(n) is the least number k such that the number of distinct digits
 *          in the prime factorization of k is n (counting terms of the form
 *          p^1 as p).
 * https://oeis.org/A254317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the smallest k whose prime factorization contains n
 *            distinct decimal digits. (by exponents all equal 1) a(1) = 1;
 *            for n > 10 the value is undefined (return 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   build_primes_upto, popcount_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/popcount_mask.h"

static bool initialized = false;
static seq_t best_for_pop[11];

static void init_sequence(void) {
    const idx_t LIMIT = 200000;          /* sufficient to cover all digits */
    uint64_t *primes;
    size_t cnt;
    build_primes_upto(LIMIT, &primes, &cnt);

    seq_t dp[1 << 10] = {0};
    dp[0] = (seq_t)1;     /* product of no primes */

    for (size_t pi = 0; pi < cnt; ++pi) {
        uint64_t p = primes[pi];
        unsigned mask = 0;
        uint64_t tmp = p;
        if (tmp == 0) continue;          /* should not happen */
        while (tmp) {
            unsigned digit = tmp % 10;
            mask |= 1u << digit;
            tmp /= 10;
        }
        for (int m = 1023; m >= 0; --m) {
            if (!dp[m]) continue;
            unsigned newm = m | mask;
            seq_t prod = dp[m] * (seq_t)p;
            if (!dp[newm] || prod < dp[newm]) dp[newm] = prod;
        }
    }

    for (int i = 0; i < 11; ++i) best_for_pop[i] = (seq_t)0;
    for (unsigned m = 1; m < 1024; ++m) {
        if (!dp[m]) continue;
        int pc = popcount_mask(m);
        if (!best_for_pop[pc] || dp[m] < best_for_pop[pc])
            best_for_pop[pc] = dp[m];
    }

    free(primes);
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset below 1: undefined */
    if (n == 1) return (seq_t)1;          /* special case */
    if (n > 10) return (seq_t)0;          /* undefined for n > 10 */

    if (!initialized) init_sequence();

    return best_for_pop[(int)n];
}
