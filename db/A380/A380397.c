/*
 * A380397  The number of nonunitary divisors of n that are cubes.
 * https://oeis.org/A380397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of nonunitary cube divisors of n. Computation:
 *            factor n, count all cubes (∏(⌊e/3⌋+1)), subtract unitary cubes
 *            (2^k where k=#primes with e≡0 mod 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void factor_uint64(uint64_t n,
                          uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) { *cnt = 0; return; }

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        size_t e = 0;
        while ((n & 1ULL) == 0) { n >>= 1; ++e; }
        primes[c] = 2ULL; exps[c] = e; ++c;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            size_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[c] = p; exps[c] = e; ++c;
        }
    }
    if (n > 1) {
        primes[c] = n; exps[c] = 1; ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n=0, negative */

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_uint64(N, primes, exps, &cnt);

    /* total cube divisors: ∏ (⌊e/3⌋ + 1) */
    seq_t total_cubes = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t add = (seq_t)(exps[i] / 3 + 1);
        total_cubes *= add;
    }

    /* unitary cube divisors: 2^k, k = #primes with e % 3 == 0 */
    seq_t unitary_cubes = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] % 3 == 0) {
            unitary_cubes <<= 1;   /* multiply by 2 */
        }
    }

    return total_cubes - unitary_cubes;
}
