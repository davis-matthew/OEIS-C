/*
 * A350329  a(n) is the least positive integer such that 2^n + a(n)*n*(n+1)
 *          equals a power of 2.
 * https://oeis.org/A350329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2, powmod2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the OEIS A350329 sequence:  
   a(n) = least positive integer such that 2^n + a(n)*n*(n+1) is a power of 2.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: pow2 is provided in utils */
#include "../utils/pow2.h"
#include "../utils/powmod2.h"

/* Sieve primes up to MAX_PRIME.  Used for factorisations. */
#define MAX_PRIME 20000
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void prepare_primes(void) {
    if (primes_ready) return;
    size_t sz = MAX_PRIME + 1;
    bool *is_composite = (bool*)calloc(sz, 1);
    primes = (uint32_t*)malloc(sz * sizeof(uint32_t));
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME)
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
        }
    }
    free(is_composite);
    primes_ready = true;
}

/* Factorisation utility: returns true if n becomes 1; fills f array with unique primes in n. */
static void factor_unique(uint32_t n, uint32_t *f, size_t *cnt) {
    size_t c = 0;
    prepare_primes();
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            f[c++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) f[c++] = n;
    *cnt = c;
}

/* Compute Euler's totient of n, along with the list of unique prime factors of phi. */
static uint32_t totient_and_factors(uint32_t n, uint32_t *phi_facts, size_t *phi_cnt) {
    uint32_t phi = n;
    uint32_t tmp = n;
    prepare_primes();
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (tmp > 1) phi = phi / tmp * (tmp - 1);
    factor_unique(phi, phi_facts, phi_cnt);
    return phi;
}

/* Compute the multiplicative order of 2 modulo m (m odd). */
static uint32_t multiplicative_order(uint32_t m) {
    if (m == 1) return 1;
    uint32_t phi_facts[32];
    size_t phi_cnt;
    uint32_t phi = totient_and_factors(m, phi_facts, &phi_cnt);
    uint32_t order = phi;
    for (size_t i = 0; i < phi_cnt; ++i) {
        uint32_t p = phi_facts[i];
        while (order % p == 0) {
            uint32_t candidate = order / p;
            if (powmod2(candidate, m) == 1)
                order = candidate;
            else
                break;
        }
    }
    return order;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n) {
    /* n >= 1 by problem definition. */
    uint64_t denom_ull = (uint64_t)n * ((uint64_t)n + 1ULL);
    /* Count trailing zeros of denom (power of 2 dividing denom). */
    uint32_t v2 = denom_ull == 0 ? 0 : __builtin_ctzll(denom_ull);
    uint32_t k = (uint32_t)(denom_ull >> v2);  /* odd part */
    uint32_t d = (k == 1) ? 1 : multiplicative_order(k);
    /* Compute a = (2^{n+d} - 2^n) / (n*(n+1)) */
    seq_t pow2_n = pow2((idx_t)n);
    seq_t pow2_nd = pow2((idx_t)(n + d));
    seq_t numerator = pow2_nd - pow2_n;
    seq_t denom_seq = (seq_t)denom_ull;   /* cast fits into seq_t */
    return numerator / denom_seq;
}
