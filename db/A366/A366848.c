/*
 * A366848  Odd numbers whose odd prime indices are relatively prime.
 * https://oeis.org/A366848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd numbers whose odd prime indices are pairwise coprime (enumerated by search) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

static bool primes_initialized = false;
static uint64_t *primes = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_limit = 2000000; /* large enough for n up to ~10000 */

/* initialize the prime table */
static void init_prime_table(void) {
    if (!primes_initialized) {
        build_primes_upto(prime_limit, &primes, &prime_cnt);
        primes_initialized = true;
    }
}

/* check if an odd number m satisfies the condition */
static bool good_odd(uint64_t m) {
    uint64_t temp = m;
    bool      has_odd_index = false;
    seq_t     gcd_indices   = 0; /* gcd(x,0)=x */

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            /* factor out p */
            while (temp % p == 0) {
                temp /= p;
            }
            /* prime index is i+1 (1‑based) */
            uint64_t idx = i + 1;
            if (idx & 1) {          /* odd index */
                has_odd_index = true;
                gcd_indices = gcd(gcd_indices, (seq_t)idx);
            }
        }
    }
    if (temp > 1) { /* remaining prime factor > sqrt(original m) */
        /* binary search for its index */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (primes[mid] == temp) {
                lo = mid;
                break;
            } else if (primes[mid] < temp) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (lo < prime_cnt && primes[lo] == temp) {
            uint64_t idx = lo + 1;
            if (idx & 1) {
                has_odd_index = true;
                gcd_indices = gcd(gcd_indices, (seq_t)idx);
            }
        }
    }
    return has_odd_index && gcd_indices == 1;
}

seq_t a(idx_t n) {
    init_prime_table();

    idx_t count = 0;
    uint64_t candidate = 3; /* start from the first odd number > 1 */

    while (true) {
        if (candidate & 1) { /* only odds */
            if (good_odd(candidate)) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
}
