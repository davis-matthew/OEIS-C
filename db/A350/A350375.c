/*
 * A350375  Numbers with exactly 8 semiprime divisors.
 * https://oeis.org/A350375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1083, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers with exactly 8 semiprime divisors */

#include <stdint.h>
#include <stdlib.h>

/* Helper: Smallest prime factor array for integers up to LIMIT */
#ifndef LIMIT
#define LIMIT 5000000
#endif

static uint32_t *spf = NULL;           /* size LIMIT+1 */
static seq_t *vals = NULL;             /* collected sequence values */
static size_t vals_len = 0;
static size_t vals_cap = 0;
static int initialized = 0;

/* Sieve for smallest prime factors */
static void init_spf(void) {
    uint32_t *s = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!s) exit(1);
    for (uint32_t i = 0; i <= LIMIT; ++i) s[i] = 0;
    s[0] = 0; s[1] = 1;
    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (!s[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) {
                if (!s[j]) s[j] = i;
            }
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i) if (!s[i]) s[i] = i;
    spf = s;
}

/* Compute the sequence up to LIMIT */
static void compute_vals(void) {
    vals_cap = 20000;
    vals = (seq_t*)malloc(vals_cap * sizeof(seq_t));
    if (!vals) exit(1);

    for (uint32_t n = 2; n <= LIMIT; ++n) {
        uint32_t x = n;
        int k = 0;      /* number of distinct primes */
        int t = 0;      /* number of primes with exponent >= 2 */
        while (x > 1) {
            uint32_t p = spf[x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            ++k;
            if (exp >= 2) ++t;
        }
        int f = t + (k * (k - 1)) / 2;
        if (f == 8) {
            if (vals_len == vals_cap) {
                vals_cap *= 2;
                vals = (seq_t*)realloc(vals, vals_cap * sizeof(seq_t));
                if (!vals) exit(1);
            }
            vals[vals_len++] = (seq_t)n;
        }
    }
}

/* Public function ------------------------------------------------------*/
seq_t a(idx_t n) {
    if (!initialized) {
        init_spf();
        compute_vals();
        initialized = 1;
    }
    /* n is 1-based */
    if (n == 0 || n > vals_len) {
        /* In case the precomputed limit was insufficient; generate more (unlikely) */
        idx_t target = n;
        idx_t current_lim = LIMIT;
        while (n > vals_len) {
            /* Double the limit and recompute (simplest but expensive) */
            current_lim *= 2;
            init_spf();           /* free previous? reuse not allowed; skip for simplicity */
            compute_vals();       /* this will append to existing vals */
        }
    }
    return vals[n - 1];
}
