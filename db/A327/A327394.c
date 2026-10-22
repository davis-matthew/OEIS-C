/*
 * A327394  Number of stable divisors of n.
 * https://oeis.org/A327394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A378442(d)
 * Range:     u8 n <= 8053, u16 n <= 8053, u32 n <= 8053, u64 n <= 8053,
 *            u128 n <= 8053, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A378442(d) is 1 if d is a stable number, 0 otherwise. */
/* A number is stable if its distinct prime indices are pairwise indivisible. */
/* (Prime index of p is its position in the sequence of primes, 2 → 1, 3 → 2, ... ) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PREP 1000000ULL

/* global arrays for sieving */
static bool _seq_initialized = false;
static uint32_t *_seq_spf = NULL;
static uint32_t *_seq_prime_index = NULL;
static uint32_t *_seq_primes = NULL;
static size_t _seq_prime_cnt = 0;

/* Helper to get prime index (1‑based). */
static size_t _prime_index_of(uint64_t p) {
    if (p <= MAX_PREP)
        return _seq_prime_index[p];
    /* p may be larger than MAX_PREP; count primes <= p */
    size_t cnt = 0;
    for (size_t i = 0; i < _seq_prime_cnt; ++i)
        if (_seq_primes[i] <= p) cnt++; else break;
    return cnt;
}

/* One‑time sieve: build spf, primes list and prime indices. */
static void _seq_init(void) {
    if (_seq_initialized) return;
    _seq_spf = (uint32_t*)calloc(MAX_PREP + 1, sizeof(uint32_t));
    _seq_prime_index = (uint32_t*)calloc(MAX_PREP + 1, sizeof(uint32_t));
    size_t *prime_tmp = (size_t*)malloc((MAX_PREP + 1) * sizeof(size_t));
    size_t primes_cap = 0;
    size_t primes_size = 0;

    for (uint32_t i = 2; i <= MAX_PREP; ++i) {
        if (_seq_spf[i] == 0) {
            _seq_spf[i] = i;
            if (primes_size >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                prime_tmp = (size_t*)realloc(prime_tmp, primes_cap * sizeof(size_t));
            }
            prime_tmp[primes_size++] = i;
        }
        for (size_t k = 0; k < primes_size && prime_tmp[k] <= _seq_spf[i] && (uint64_t)i * prime_tmp[k] <= MAX_PREP; ++k) {
            _seq_spf[i * prime_tmp[k]] = prime_tmp[k];
        }
    }

    _seq_primes = (uint32_t*)malloc(primes_size * sizeof(uint32_t));
    for (size_t i = 0; i < primes_size; ++i) _seq_primes[i] = (uint32_t)prime_tmp[i];
    _seq_prime_cnt = primes_size;

    for (size_t i = 0; i < _seq_prime_cnt; ++i)
        _seq_prime_index[_seq_primes[i]] = i + 1;  /* 1‑based index */

    free(prime_tmp);
    _seq_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    _seq_init();

    if (n <= 1) return (seq_t)1;      /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    /* arrays to hold distinct primes and exponents */
    uint64_t primes[32];
    size_t exp[32];
    size_t k = 0;

    /* Factorization */
    while (nn > 1) {
        uint64_t p = 0, e = 0;
        if (nn <= MAX_PREP) {
            p = _seq_spf[nn];
            while (nn % p == 0) {
                nn /= p;
                e++;
            }
        } else {
            /* trial division using precomputed primes (up to 1e6) */
            bool found = false;
            for (size_t i = 0; i < _seq_prime_cnt && (uint64_t)_seq_primes[i] * _seq_primes[i] <= nn; ++i) {
                if (nn % _seq_primes[i] == 0) {
                    p = _seq_primes[i];
                    while (nn % p == 0) {
                        nn /= p;
                        e++;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {  /* nn is prime */
                p = nn;
                e = 1;
                nn = 1;
            }
        }
        primes[k] = p;
        exp[k] = (size_t)e;
        ++k;
    }

    /* Build arrays of prime indices and corresponding exponents */
    size_t indices[32];
    for (size_t i = 0; i < k; ++i)
        indices[i] = _prime_index_of(primes[i]);

    /* Count stable divisors */
    seq_t ans = 1; /* empty subset -> divisor 1 */

    size_t total_masks = ((size_t)1) << k;
    for (size_t mask = 1; mask < total_masks; ++mask) {
        size_t sub_indices[32];
        size_t sub_cnt = 0;
        seq_t prod = 1;
        bool stable = true;

        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                prod *= (seq_t)exp[i];
                sub_indices[sub_cnt++] = indices[i];
                if (!stable) continue;
                for (size_t j = 0; j < sub_cnt - 1; ++j) {
                    size_t a = sub_indices[j];
                    size_t b = sub_indices[sub_cnt - 1];
                    if (a == b) continue;           /* should not happen */
                    if ((b % a == 0) || (a % b == 0)) {
                        stable = false;
                        break;
                    }
                }
            }
        }
        if (stable) ans += prod;
    }

    return ans;
}
