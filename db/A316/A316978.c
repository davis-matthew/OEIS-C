/*
 * A316978  Number of factorizations of n into factors > 1 with no equivalent
 *          primes.
 * https://oeis.org/A316978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates multiplicative partitions of n into factors > 1 that
 * contain no two equivalent primes.  The simple recursive generator
 * uses the factorise function from ../utils/factor_u64.h. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint8_t patterns[12][20];
static uint32_t primes[12];
static int      prime_count;

/* Recursively count factorizations of the remaining product `rem`.  
 * `min_factor` ensures we only consider non‑decreasing factor order,
 * preventing permutations.  `depth` records how many factors have already
 * been chosen.  `patterns` stores, for each prime of the original n,
 * the exponent of that prime in each chosen factor. */
static seq_t rec(uint64_t rem, uint64_t min_factor, int depth)
{
    if (rem == 1) {
        if (depth == 0) return 1;        /* only n == 1 */
        /* check for equivalent primes: any two primes with identical
         * exponent patterns across all factors invalidate the
         * factorization. */
        for (int i = 0; i < prime_count; ++i)
            for (int j = i + 1; j < prime_count; ++j) {
                int k;
                for (k = 0; k < depth; ++k)
                    if (patterns[i][k] != patterns[j][k]) break;
                if (k == depth)              /* equal pattern */
                    return 0;                /* forbidden factorization */
            }
        return 1;                           /* valid factorization */
    }

    seq_t total = 0;

    /* One‑factor branch: use the remaining product itself as a factor. */
    if (rem >= min_factor) {
        for (int i = 0; i < prime_count; ++i) {
            uint64_t t = rem;
            uint8_t  e = 0;
            uint32_t p = primes[i];
            while (t % p == 0) { ++e; t /= p; }
            patterns[i][depth] = e;
        }
        total += rec(1, rem, depth + 1);
    }

    /* Proper divisors – keep factors in non‑decreasing order. */
    for (uint64_t d = min_factor; d * d <= rem; ++d) {
        if (rem % d == 0) {
            uint64_t q = rem / d;
            for (int i = 0; i < prime_count; ++i) {
                uint64_t t = d;
                uint8_t  e = 0;
                uint32_t p = primes[i];
                while (t % p == 0) { ++e; t /= p; }
                patterns[i][depth] = e;
            }
            total += rec(q, d, depth + 1);
        }
    }

    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Prime factorise n.  The maximum number of distinct primes below 10000
     * is 5, so 12 slots provide ample room. */
    uint64_t      tmp = (uint64_t)n;
    size_t        cnt = 0;
    uint64_t        pr[12], ex[12];
    factor_u64(tmp, pr, ex, &cnt);
    prime_count = (int)cnt;
    for (int i = 0; i < prime_count; ++i) primes[i] = (uint32_t)pr[i];

    /* Clear the pattern table for the new call. */
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 20; ++j)
            patterns[i][j] = 0;

    return rec(tmp, 2, 0);
}
