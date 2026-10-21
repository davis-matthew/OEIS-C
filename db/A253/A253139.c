/*
 * A253139  a(n) = lcm_{d|n} tau(d), where tau(d) represents the number of
 *          divisors of d (A000005(d)).
 * https://oeis.org/A253139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = lcm_{d|n} tau(d)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Compute the least common multiple of 1..k for k up to 64 */
static seq_t lcm_table[66];
static int lcm_initialized = 0;

static void init_lcm_table(void) {
    lcm_table[0] = 1;
    lcm_table[1] = 1;
    for (idx_t i = 2; i < 66; ++i) {
        seq_t prev = lcm_table[i-1];
        seq_t g = gcd(prev, (seq_t)i);
        lcm_table[i] = prev / g * (seq_t)i;
    }
    lcm_initialized = 1;
}

seq_t a(idx_t n) {
    if (!lcm_initialized)
        init_lcm_table();

    if (n == 1)
        return (seq_t)1;

    /* Factorize n.  For uint64_t max distinct primes <= 16. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        /* exp+1 <= 64 for uint64_t n */
        result = result * lcm_table[(size_t)exp + 1];
    }
    return result;
}
