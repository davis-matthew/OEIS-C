/*
 * A301517  Numbers whose ratio (sum of nonsquarefree divisors)/(sum of
 *          squarefree divisors) is a positive integer.
 * https://oeis.org/A301517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 473, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A301517 – Numbers whose ratio (sum of non‑square‑free divisors)/(sum of 
   square‑free divisors) is a positive integer. 
   The function returns the n‑th positive integer m that satisfies this
   property. The implementation builds the sequence on demand, storing
   found values in a dynamically sized array. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum integer for which we precompute a smallest‑prime‑factor sieve. 
   10 million is enough for the first ten thousand terms.               */
#define NMAX 10000000ULL
#define MAX_PRIMES (NMAX/10)

/* Global tables for factoring.  They are filled only once. */
static uint32_t spf[NMAX + 1];
static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool sieve_done = false;

/* Dynamic storage for found sequence terms. */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Highest integer that has been tested for membership. */
static uint64_t tested_max = 1;

/* Small power routine for integer bases */
static inline seq_t seq_pow_uint(uint64_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1U) result *= b;
        if (exp > 1) b *= b;
        exp >>= 1U;
    }
    return result;
}

/* Build sieve of smallest prime factors and prime list. */
static void init_sieve(void) {
    if (sieve_done) return;

    /* Initialize spf. */
    for (size_t i = 0; i <= NMAX; ++i) spf[i] = (uint32_t)i;

    /* Sieve. */
    for (size_t i = 2; (uint64_t)i * i <= NMAX; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (uint64_t j = (uint64_t)i * i; j <= NMAX; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Build prime list. */
    for (size_t i = 2; i <= NMAX; ++i) {
        if (spf[i] == (uint32_t)i) {
            if (prime_cnt < MAX_PRIMES) primes[prime_cnt++] = (uint32_t)i;
        }
    }
    sieve_done = true;
}

/* Ensure dynamic array capacity. */
static void ensure_seq_capacity(size_t needed) {
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    /* Reallocate with an explicit cast to satisfy both C and C++. */
    seq_t *newp = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    if (!newp) { /* memory exhausted */ abort(); }
    seq_arr = newp;
    seq_cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */

    init_sieve();          /* build tables once */

    /* Compute terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++tested_max;
        if (tested_max <= 1) continue;   /* skip 1 – ratio is 0 */

        uint64_t tmp = tested_max;
        seq_t sigma = 1;      /* product of sigma factors */
        seq_t sum_sf = 1;     /* product of (1+p) over distinct primes */

        /* Factorization using the precomputed spf table. */
        while (tmp > 1) {
            uint64_t p = spf[tmp];
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* sigma factor = (p^(e+1)-1)/(p-1) */
            seq_t powp = seq_pow_uint(p, e + 1);
            seq_t numerator = powp - 1;
            seq_t denom = (seq_t)(p - 1);
            seq_t sigma_factor = numerator / denom;
            sigma *= sigma_factor;
            sum_sf *= (seq_t)(p + 1);
        }

        seq_t sum_ns = sigma - sum_sf; /* sum of nonsquarefree divisors */
        if (sum_ns > 0 && sum_ns % sum_sf == 0) {
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len] = tested_max;
            ++seq_len;
        }
    }
    return seq_arr[n - 1];
}
