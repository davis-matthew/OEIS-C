/*
 * A302243  Total weight of the n-th twice-odd-factored multiset partition.
 * https://oeis.org/A302243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A302242(2n+1) A302242(2n+1) = Σ_{p^e‖(2n+1)} e * Ω(π(p))
 *            where π(p) is the 1‑based index of prime p in the prime list and
 *            Ω(k) is the total number of prime factors of k (counting
 *            multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state */
static bool      initialized = false;
static uint64_t  max_limit    = 0;      /* maximum m for which tables exist */
static uint32_t *primes       = NULL;  /* list of primes <= max_limit */
static size_t   prime_cnt      = 0;
static uint32_t *prime_index  = NULL;  /* prime_index[p] = 1‑based index of p, 0 if not prime */
static seq_t   *omega         = NULL;  /* omega[k] = Ω(k) for k = 1 .. prime_cnt */

/* ------------------------------------------------------------------ */
/* build primes up to “limit” and the related tables                  */
static void
ensure_primes(uint64_t limit)
{
    if (initialized && limit <= max_limit)
        return;                 /* up to date */

    /* free old tables */
    free(primes);
    free(prime_index);
    free(omega);

    /* --- sieve of Eratosthenes up to limit --- */
    uint64_t size = limit + 1;
    uint8_t  *is_comp = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (!is_comp) return;          /* out of memory, caller will get zero */

    prime_cnt = 0;
    /* rough upper bound for number of primes <= limit (prime number theorem) */
    size_t probable = (size > 5) ? (size / 4) : 5;
    primes = (uint32_t *)malloc(probable * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (prime_cnt == probable) {
                probable *= 2;
                primes = (uint32_t *)realloc(primes, probable * sizeof(uint32_t));
                if (!primes) break;
            }
            primes[prime_cnt++] = (uint32_t)p;
            if (p * p <= limit) {
                for (uint64_t j = p * p; j <= limit; j += p)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    /* allocate prime_index array */
    prime_index = (uint32_t *)calloc(size, sizeof(uint32_t));
    if (!prime_index) { free(primes); return; }
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index[primes[i]] = (uint32_t)(i + 1); /* 1‑based index */

    /* compute Ω(k) for k = 1 .. prime_cnt */
    omega = (seq_t *)calloc(prime_cnt + 1, sizeof(seq_t));
    if (!omega) { free(primes); free(prime_index); return; }

    /* smallest prime factor array for 1 .. prime_cnt */
    uint32_t *spf = (uint32_t *)calloc(prime_cnt + 1, sizeof(uint32_t));
    if (!spf) { free(primes); free(prime_index); free(omega); return; }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        for (uint32_t k = p; k <= prime_cnt; k += p)
            if (!spf[k])
                spf[k] = p;
    }
    omega[1] = 0;
    for (uint32_t k = 2; k <= prime_cnt; ++k)
        omega[k] = omega[k / spf[k]] + 1;
    free(spf);

    max_limit    = limit;
    initialized  = true;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                           */
seq_t a(idx_t n)
{
    uint64_t m = ((uint64_t)n) * 2 + 1;   /* 2n+1 fits in uint64_t for all idx_t */
    if (m < 2) /* m == 1 for n == 0 */
        return 0;

    /* build tables up to m */
    if (!initialized || m > max_limit)
        ensure_primes(m);

    seq_t sum = 0;
    uint64_t tmp = m;

    /* trial division using the precomputed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp)
            break;
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            uint32_t idx = prime_index[p];            /* p is prime => idx >=1 */
            sum += (seq_t)exp * omega[idx];
        }
    }

    if (tmp > 1) {                 /* remaining prime factor */
        uint32_t idx = prime_index[(uint32_t)tmp];
        sum += omega[idx];
    }

    return sum;
}
