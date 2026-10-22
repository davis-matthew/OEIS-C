/*
 * A347101  Fully multiplicative with a(prime(k)) = A001223(k), where A001223
 *          gives the distance from the k-th prime to the (k+1)-th prime.
 * https://oeis.org/A347101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 2400, u16 n <= 9005, u32 n <= 9005, u64 n <= 9005,
 *            u128 n <= 9005, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347101
   Fully multiplicative with a(p_k) = prime_gap_k,
   where prime_gap_k = p_{k+1} – p_k (distance between the
   k‑th prime and the (k+1)-th prime).
   a(1) = 1   and   a(2^n) = 1 for all n ≥ 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* sieve limit – enough for factorising n up to ~4·10¹² (√n ≤ 2·10⁶) */
#define PRIME_LIMIT 2000000

static uint32_t primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* generate all primes ≤ PRIME_LIMIT once per program */
static void init_primes(void)
{
    if (primes_inited) return;
    static unsigned char is_composite[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_composite[i] = 0;
    is_composite[0] = is_composite[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                is_composite[(size_t)j] = 1;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) primes[prime_cnt++] = i;
    primes_inited = true;
}

/* multiplicative evaluation of a(n) */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    init_primes();

    idx_t m = n;
    seq_t result = (seq_t)1;

    /* trial division using the precomputed primes */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)m) break;
        if (m % p == 0) {
            /* count exponent of prime p */
            uint32_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* compute prime gap for this prime factor */
            seq_t gap = (seq_t)(next_prime(p) - p);
            /* multiply result by gap^exp */
            for (uint32_t k = 0; k < exp; ++k) result *= gap;
        }
    }

    /* any remaining factor > sqrt(original n) is prime itself */
    if (m > 1) {
        seq_t gap = (seq_t)(next_prime((uint64_t)m) - (uint64_t)m);
        result *= gap;   /* exponent is 1 */
    }

    return result;
}
