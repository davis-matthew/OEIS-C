/*
 * A387114  Number of divisors in common to all prime indices of n.
 * https://oeis.org/A387114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A289508(n)) - the number of divisors of the *
 *            greatest common divisor of the indices of the prime factors of
 *            n. * For n=1, a(1)=0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, divisor_count_u64, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static uint64_t prime_lim = 0;

seq_t a(idx_t n) {
    if (n <= 1) return 0;  // a(1) = 0

    uint64_t x = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t num;
    factor_u64(x, primes, exps, &num);

    // Determine the largest prime factor
    uint64_t maxp = 0;
    for (size_t i = 0; i < num; ++i)
        if (primes[i] > maxp) maxp = primes[i];

    // Rebuild prime table if needed
    if (maxp > prime_lim) {
        free(prime_list);
        build_primes_upto(maxp, &prime_list, &prime_cnt);
        prime_lim = maxp;
    }

    // Compute gcd of the prime indices
    seq_t g = 0;
    for (size_t i = 0; i < num; ++i) {
        uint64_t p = primes[i];
        /* binary search for the index of p */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        seq_t idx = (seq_t)(lo + 1); /* indices are 1‑based */
        g = (i == 0) ? idx : gcd(g, idx);
    }

    return (seq_t)divisor_count_u64((uint64_t)g);
}
