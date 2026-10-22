/*
 * A339927  Number of partitions of n into two composite parts with the same
 *          number of divisors.
 * https://oeis.org/A339927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..⌊n/2⌋} [d(k)=d(n-k)] * c(k) * c(n-k) where d(m)
 *            = number of divisors of m and c(m) = 1 if m is composite, 0
 *            otherwise
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static storage for sieve data */
static uint32_t *spf = NULL;   /* smallest prime factor */
static uint16_t *divcnt = NULL;   /* divisor counts d(n)   */
static bool *is_comp = NULL;  /* composite flag c(n)  */
static idx_t max_sieve = 0;   /* maximum index computed so far */

/* ensure sieve data up to at least n */
static void ensure_sieve(idx_t n) {
    if (n <= max_sieve) return;          /* already computed */

    /* resize arrays */
    size_t new_cap = (size_t)n + 1;
    spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    divcnt = (uint16_t *)realloc(divcnt, new_cap * sizeof(uint16_t));
    is_comp = (bool *)realloc(is_comp, new_cap * sizeof(bool));
    if (!spf || !divcnt || !is_comp) abort();

    /* zero out new entries (old data will be recomputed) */
    memset(spf, 0, new_cap * sizeof(uint32_t));
    memset(divcnt, 0, new_cap * sizeof(uint16_t));
    memset(is_comp, 0, new_cap * sizeof(bool));

    /* linear sieve up to n */
    idx_t *primes = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!primes) abort();
    size_t pc = 0;          /* number of primes found */

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (p > spf[i] || ip > n) break;
            spf[ip] = (uint32_t)p;
        }
    }
    free(primes);

    /* compute divisor counts and composite flags */
    if (n >= 1) {
        divcnt[1] = 1;
        is_comp[0] = false;
        is_comp[1] = false;
    }
    for (idx_t i = 2; i <= n; ++i) {
        uint32_t x = (uint32_t)i;
        uint16_t prod = 1;
        while (x > 1) {
            uint32_t p = spf[x];
            uint16_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            prod *= (exp + 1);
        }
        divcnt[i] = prod;
        is_comp[i] = (spf[i] != i);   /* composite if not prime */
    }

    max_sieve = n;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(1) = 0 */

    ensure_sieve(n);              /* make sure data for 2..n are ready */

    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t k = 2; k <= half; ++k) {
        idx_t j = n - k;
        if (j < 2) continue;
        if (is_comp[k] && is_comp[j] && divcnt[k] == divcnt[j]) {
            result++;              /* ++ works for arbitrary-precision types */
        }
    }
    return result;
}
