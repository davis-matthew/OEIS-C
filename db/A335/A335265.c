/*
 * A335265  a(n) = Denominator(-4*n^2*Zeta(1 - n)^2*(1 - 2^n)) for n >= 1,
 *          a(0) = 1.
 * https://oeis.org/A335265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(Bernoulli(n)^2*(2^(n+2) - 4))
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 1, a(4) = 15, ... */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* Modulo multiplication using unsigned __int128 to avoid overflow */

/* Fast modular exponentiation: returns (base^exp) % mod */
static uint64_t modpow_u64(uint64_t base, idx_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t power = base % mod;
    while (exp > 0) {
        if (exp & 1U) result = mul_mod64(result, power, mod);
        power = mul_mod64(power, power, mod);
        exp >>= 1;
    }
    return result;
}

/* Precompute primes up to a fixed limit (enough for n <= 10000) */
#define PRIME_LIMIT 20004   /* > 10001 */
static idx_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    static char comp[PRIME_LIMIT + 1];
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) comp[i] = 0;
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) comp[j] = 1;
        }
    }
    primes_cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) primes[primes_cnt++] = i;
    }
    primes_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (!primes_inited) init_primes();

    if (n == 0) return (seq_t)1;      /* a(0) = 1 */

    seq_t result = 1;                 /* accumulate product of odd primes */

    /* iterate over primes p <= n+1 */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        idx_t p = primes[idx];
        if (p > n + 1) break;
        if (n % (p - 1) != 0) continue;      /* p does not satisfy p-1 | n */

        if (p == 2) continue;                /* factor 2 cancels completely */

        uint64_t mod = (uint64_t)p * (uint64_t)p;
        uint64_t pow = modpow_u64(2, n, mod);

        if (pow == 1) continue;              /* p^2 divides d, cancel entirely */

        result *= (seq_t)p;                   /* include odd prime once */
    }

    return result;
}
