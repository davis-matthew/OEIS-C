/*
 * A374672  Numbers k such that k! has more infinitary divisors than (k+1)!.
 * https://oeis.org/A374672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374672: numbers k such that k! has more infinitary divisors
   than (k+1)!.  The sequence is computed by iterating k, maintaining the
   exponents of prime factors in k! and the sum of bit counts of these
   exponents.  A item k is included when the sum decreases from k! to
   (k+1)! (i.e. τ_inf(k!) > τ_inf((k+1)!)). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/popcnt_uint32.h"

/* ----------------- static state ------------------- */
static const size_t PRIME_LIMIT = 3000000;    /* sieve size, > max k needed */

/* prime list */
static uint32_t *primes = NULL;      /* primes[i] = i-th prime (0‑based)  */
static size_t primes_cap = 0;        /* number of primes stored */

/* exponent of each prime in the current k! */
static uint32_t *prime_exp = NULL;   /* prime_exp[i] = exponent of primes[i] */

/* current factorial index */
static idx_t cur_k = 1;              /* current processed k, starting at 1 */

/* current sum of popcounts of exponents (S_k) */
static seq_t cur_S = 0;

/* memoised terms of the sequence */
static seq_t *terms = NULL;          /* array of found k values */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of terms array */

/* ----------------- sieve and initialization ------------------ */
static void init_sieve(void)
{
    /* Sieve of Eratosthenes for primes up to PRIME_LIMIT */
    uint8_t *is_prime = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) abort();

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes and allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    primes_cap = cnt;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);

    /* allocate exponent array, zero initialized */
    prime_exp = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));
    if (!prime_exp) abort();
}

/* binary search for index of prime p in primes array */
static inline size_t find_prime_index(uint32_t p)
{
    size_t lo = 0, hi = primes_cap;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t midp = primes[mid];
        if (midp == p) return mid;
        if (midp < p) lo = mid + 1;
        else hi = mid;
    }
    return primes_cap; /* should not happen if p <= PRIME_LIMIT */
}

/* grow the terms array when needed */
static inline void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
    } else if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) abort();
    }
}

/* ----------------- main sequence function -------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* offset is 1 */

    /* one‑time initialization */
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        idx_t next_k = cur_k + 1;
        seq_t S_next = cur_S;   /* start from current sum */

        /* factorize next_k using stored primes */
        idx_t tmp = next_k;
        for (size_t i = 0; i < primes_cap && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
            uint32_t p = primes[i];
            if (tmp % p != 0) continue;
            uint32_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            uint32_t old_e = prime_exp[i];
            uint32_t new_e = old_e + cnt;
            int delta = popcnt_uint32(new_e) - popcnt_uint32(old_e);
            S_next += delta;
            prime_exp[i] = new_e;
        }

        /* if any remaining factor > sqrt(tmp) it must be prime */
        if (tmp > 1) {
            uint32_t p = (uint32_t)tmp;
            size_t idx = find_prime_index(p);
            if (idx == primes_cap) abort(); /* out of range */
            uint32_t old_e = prime_exp[idx];
            uint32_t new_e = old_e + 1;
            int delta = popcnt_uint32(new_e) - popcnt_uint32(old_e);
            S_next += delta;
            prime_exp[idx] = new_e;
        }

        /* if current_k! has more infinitary divisors than (k+1)! */
        if (cur_S > S_next) {
            grow_terms();
            terms[terms_len++] = (seq_t)cur_k;
        }

        /* move to next k */
        cur_k = next_k;
        cur_S = S_next;
    }

    return terms[n - 1];
}
