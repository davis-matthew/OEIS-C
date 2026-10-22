/*
 * A376555  The number of unitary square divisors of binomial(n, floor(n/2)).
 * https://oeis.org/A376555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unitary square divisors of binomial(n,
 *            floor(n/2))
 * Range:     u8 n <= 959, u16 n <= 6926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of unitary square divisors equals 2^k, where k is the number of primes
   dividing the binomial coefficient with an even positive exponent.  We compute
   the exponents via Legendre's formula and count only primes with e>0 and e even. */

#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_list = NULL;   /* array of primes (up to last generated n) */
static size_t   prime_cap   = 0;      /* capacity of prime_list */
static size_t   prime_cnt   = 0;      /* number of primes stored */

/* Simple sieve to generate all primes <= limit, extending prime_list if necessary. */
static void generate_primes_up_to(idx_t limit) {
    if (limit < 2) return;
    if (limit >= prime_cap) {
        size_t new_cap = limit + 1;
        uint8_t *sieve = (uint8_t*)malloc(new_cap);
        if (!sieve) return;                 /* abort silently if memory unavailable */
        for (size_t i = 0; i < new_cap; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (sieve[i]) {
                for (size_t j = i * i; j <= limit; j += i) sieve[j] = 0;
            }
        }
        uint64_t *old = prime_list;
        size_t old_cnt = prime_cnt;
        prime_list = (uint64_t*)malloc(sizeof(uint64_t) * new_cap);
        if (!prime_list) {
            free(sieve);
            if (old) free(old);
            return;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (sieve[i]) {
                prime_list[idx++] = i;
            }
        }
        prime_cnt = idx;
        prime_cap = new_cap;
        free(sieve);
        if (old) free(old);
    }
}

/* Legendre exponent of prime p in n! */
static seq_t legendre_exponent(idx_t n, uint64_t p) {
    seq_t e = 0;
    idx_t pk = p;
    while (pk <= n) {
        e += n / pk;
        if (pk > n / p) break;
        pk *= p;
    }
    return e;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* generate primes up to n (all primes that could appear in the binomial) */
    generate_primes_up_to(n);

    idx_t k = n / 2;          /* floor(n/2) */
    idx_t n_minus_k = n - k;  /* ceil(n/2) for even n */

    size_t even_prime_exponents = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n) break;
        seq_t e_n  = legendre_exponent(n, p);
        seq_t e_k  = legendre_exponent(k, p);
        seq_t e_nk = legendre_exponent(n_minus_k, p);
        seq_t e = e_n - e_k - e_nk;
        if (e > 0 && (e & 1) == 0) even_prime_exponents++;
    }

    seq_t result = 1;
    for (size_t i = 0; i < even_prime_exponents; ++i) {
        result *= (seq_t)2;
    }
    return result;
}
