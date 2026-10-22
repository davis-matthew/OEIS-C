/*
 * A338713  Number of numbers of the form i*n with 1 <= i <= n and tau(i*n) =
 *          4.
 * https://oeis.org/A338713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ i | 1 ≤ i ≤ n, τ(i·n) = 4 } (τ = number of divisors)
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* primes up to 10000 (√(10⁸)) */
#define MAX_PRIME 10000
static idx_t primes[MAX_PRIME];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized) return;
    bool is_prime[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= MAX_PRIME; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= MAX_PRIME; q += p)
                is_prime[q] = false;
        }
    }
    for (size_t p = 2; p <= MAX_PRIME; ++p) {
        if (is_prime[p]) {
            primes[primes_cnt++] = p;
        }
    }
    primes_initialized = true;
}

/* tests whether m has exactly four divisors */
static bool has_four_divisors(uint64_t m) {
    if (m < 2) return false;          /* τ(1) = 1 */

    init_primes();

    /* find smallest prime factor */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        uint64_t p2 = p * p;
        if (p2 > m) break;
        if (m % p == 0) {
            /* count exponent of p */
            int exp = 0;
            uint64_t tmp = m;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp == 3 && tmp == 1)  /* p^3 */
                return true;
            if (exp == 1) {
                /* m = p * tmp, tmp must be a prime ≠ p */
                if (tmp == 1 || tmp == p) return false;
                /* primality test for tmp */
                for (size_t j = 0; j < primes_cnt; ++j) {
                    uint64_t q = primes[j];
                    uint64_t q2 = q * q;
                    if (q2 > tmp) break;
                    if (tmp % q == 0) return false;
                }
                return true;
            }
            return false;   /* exp > 3 or exp == 2 → τ > 4 */
        }
    }
    /* prime > 10000 -> no
       but such m would be > 10⁸, not possible in our range */
    return false;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    /* iterate i from 1 to n inclusive */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t m = (uint64_t)n * (uint64_t)i;
        if (has_four_divisors(m)) ++count;
    }
    return count;
}
