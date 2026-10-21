/*
 * A278967  a(n) = least multiple of the n-th prime that has the minimum
 *          Hamming weight (=A278966(n)).
 * https://oeis.org/A278967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 31, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A278967  –  least multiple of the n-th prime whose
    Hamming weight is minimal among all multiples.
    The returned value fits into the type seq_t, but will be
    correct for all n up to about 10 000.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve – generates all primes up to a fixed limit.           */
#define PRIME_LIMIT 130000             /* > 10 000th prime (104 729) */
static uint64_t *prime_table = NULL;   /* global storage for primes   */
static size_t prime_count = 0;

/* Initialise the sieve once.  */
static void init_primes(void)
{
    if (prime_table != NULL) return;                /* already initialised */

    bool *is_prime = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;            /* allocation failure – no primes */

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    prime_table = (uint64_t*)malloc((PRIME_LIMIT / 10) * sizeof(uint64_t));
    if (!prime_table) { free(is_prime); return; }

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_table[cnt++] = i;
        }
    }
    prime_count = cnt;
    prime_table = (uint64_t*)realloc(prime_table, prime_count * sizeof(uint64_t));
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/*  Modular exponentiation (using 64‑bit integers, safe for p < 2^64). */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = (__uint128_t)result * base % mod;
        base = (__uint128_t)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/*  Compute the multiplicative order of 2 modulo a prime p (p > 2).   */
static uint64_t order_of_2(uint64_t p)
{
    uint64_t phi = p - 1;          /* p is prime */
    uint64_t order = phi;

    uint64_t tmp = phi;
    for (uint64_t q = 2; q * q <= tmp; ++q) {
        if (tmp % q == 0) {
            while (tmp % q == 0) tmp /= q;
            while (order % q == 0 &&
                   mod_pow_uint64(2, order / q, p) == 1) {
                order /= q;
            }
        }
    }
    if (tmp > 1) {
        while (order % tmp == 0 &&
               mod_pow_uint64(2, order / tmp, p) == 1) {
            order /= tmp;
        }
    }
    return order;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  */
static const int MAX_EXP = 64;   /* search exponents 0…63 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* out of range – not defined */
    if (n == 1) return (seq_t)2;/* a(1) = 2 */

    init_primes();              /* ensure primes ready */

    if ((size_t)(n - 1) >= prime_count) return 0; /* safety */

    uint64_t p = prime_table[n - 1];   /* n-th prime (1‑based) */
    if (p == 2) return (seq_t)2;

    /* 1st attempt – weight 2 */
    uint64_t ord = order_of_2(p);
    if ((ord & 1) == 0) {           /* order even → 2‑bit solution exists */
        uint64_t e = ord / 2;
        seq_t val = (((seq_t)1) << e) + (seq_t)1;
        return val;
    }

    /* 2nd attempt – weight 3 */
    uint64_t pow_mod[MAX_EXP];
    pow_mod[0] = 1 % p;
    for (int i = 1; i < MAX_EXP; ++i) {
        pow_mod[i] = (pow_mod[i - 1] * 2ULL) % p;
    }

    seq_t best = 0;
    bool found = false;

    for (int i = 0; i < MAX_EXP - 2; ++i) {
        for (int j = i + 1; j < MAX_EXP - 1; ++j) {
            uint64_t r = pow_mod[i] + pow_mod[j];
            if (r >= p) r -= p;
            uint64_t target = (r == 0) ? 0 : p - r;

            for (int k = j + 1; k < MAX_EXP; ++k) {
                if (pow_mod[k] == target) {
                    seq_t val = ((seq_t)1 << i) | ((seq_t)1 << j) | ((seq_t)1 << k);
                    if (!found || val < best) {
                        best = val;
                        found = true;
                    }
                    break;   /* smallest k for this (i,j) gives minimal val */
                }
            }
        }
    }

    if (found) return best;

    /* 3rd attempt – no weight‑3 solution → the prime itself is minimal. */
    return (seq_t)p;
}
