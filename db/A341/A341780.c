/*
 * A341780  Starts of runs of 3 consecutive anti-tau numbers (A046642).
 * https://oeis.org/A341780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A341780: a(n) is the start of a run of 3 consecutive
   anti‑tau numbers.  Each term is of the form 4*k^2 − 1 where
   4*k^2 − 1, 4*k^2 and 4*k^2 + 1 all satisfy gcd(x, τ(x)) = 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/gcd.h"

static uint64_t *primes   = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;
static bool     primes_ready = false;

/* Build a list of primes up to 200 000 – sufficient for k up to ~10 000. */
static void init_primes(void) {
    if (primes_ready) return;
    const idx_t SIEVE_LIMIT = 200000;   /* > 2*sqrt(max(k)) needed */

    /* Simple sieve of Eratosthenes */
    char *comp = (char*)malloc(SIEVE_LIMIT + 1);
    if (!comp) exit(1);
    for (int i = 0; i <= SIEVE_LIMIT; ++i) comp[i] = 0;
    for (int i = 2; (idx_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!comp[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                comp[j] = 1;
        }
    }

    /* Count primes and allocate array */
    size_t cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) if (!comp[i]) ++cnt;
    primes_cap = cnt;
    primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) exit(1);
    primes_cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!comp[i]) primes[primes_cnt++] = (uint64_t)i;
    }
    free(comp);
    primes_ready = true;
}

/* Compute τ(n) for a 64‑bit integer using the pre‑computed primes. */
static inline seq_t calc_tau(uint64_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t result = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        unsigned exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp) result *= (seq_t)(exp + 1);
    }
    if (tmp > 1) result *= 2;  /* remaining prime factor */
    return result;
}

/* Test whether x is anti‑tau: gcd(x, τ(x)) = 1. */
static inline bool is_anti_tau(uint64_t x) {
    seq_t t = calc_tau(x);
    return gcd((seq_t)x, t) == 1;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    init_primes();

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        /* Use 128‑bit intermediate to avoid overflow of 4*k*k. */
        unsigned __int128 temp = (unsigned __int128)4 * k * k;
        if (temp > (unsigned __int128)UINT64_MAX - 2)
            break;   /* no larger values fit in uint64_t */

        uint64_t val = (uint64_t)temp - 1; /* 4*k^2 - 1 */
        if (!is_anti_tau(val))           continue;
        if (!is_anti_tau(val + 1))       continue;
        if (!is_anti_tau(val + 2))       continue;

        ++found;
        if (found == n) return (seq_t)val;
    }
    return 0; /* Should not happen for valid n */
}
