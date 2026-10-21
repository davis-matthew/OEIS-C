/*
 * A275641  Expansion of (1-q)^k/Product_{j=1..k} (1-q^j) for k=7.
 * https://oeis.org/A275641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of q^n in (1-q)^7 / ∏_{j=1..7}(1-q^j)
 * Range:     i8 n <= 30, i16 n <= 614, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration - the required function */
seq_t a(idx_t n);

/* Internal tables and cache */
static seq_t coeff_d[29];
static seq_t coeff_n[8];
static bool initialized = false;
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* Initialise tables and first 28 terms */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Compute the denominator coefficients D(q) = Π_{j=1..7}(1 - q^j) */
    seq_t d[29] = {0};
    d[0] = 1;
    for (int j = 1; j <= 7; j++) {
        for (int k = 28 - j; k >= 0; k--) {
            d[k + j] = d[k + j] - d[k];
        }
    }
    for (int i = 0; i <= 28; i++) coeff_d[i] = d[i];

    /* Numerator coefficients N_k = (-1)^k * C(7,k) for 0 ≤ k ≤ 7 */
    const seq_t binom[8] = {1, 7, 21, 35, 35, 21, 7, 1};
    for (int k = 0; k <= 7; k++) {
        seq_t val = binom[k];
        if (k & 1) val = -val;
        coeff_n[k] = val;
    }

    /* Allocate cache, compute first 28 terms */
    cache_cap = 30;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);

    for (size_t n = 0; n < 28; n++) {
        seq_t sum = 0;
        for (int j = 1; j <= 28; j++) {
            if (n >= (size_t)j) sum = sum + coeff_d[j] * cache[n - j];
        }
        seq_t Nterm = 0;
        if (n <= 7) Nterm = coeff_n[n];
        cache[n] = Nterm - sum;
    }
    cache_size = 28;
}

/* Extend cache up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (!initialized) init_sequence();
    size_t req = (size_t)n + 1;
    if (req <= cache_size) return;

    if (req > cache_cap) {
        size_t new_cap = req * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_size; i < req; i++) {
        seq_t sum = 0;
        for (int j = 1; j <= 28; j++) {
            if (i >= (size_t)j) sum = sum + coeff_d[j] * cache[i - j];
        }
        seq_t Nterm = 0;
        if (i <= 7) Nterm = coeff_n[i];
        cache[i] = Nterm - sum;
    }
    cache_size = req;
}

/* Public function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
