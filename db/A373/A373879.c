/*
 * A373879  Composite numbers not factorizable using the Pollard-rho algorithm
 *          with parameters x=2,y=2 and f(x)=x^2-1.
 * https://oeis.org/A373879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite numbers not factorizable using the Pollard-rho algorithm
   with parameters x = 2, y = 2 and f(x) = x² - 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* gcd for 64‑bit unsigned integers */

/* modular multiplication avoiding overflow */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* f(x) = x² - 1 (mod n) */
static inline uint64_t f_mod(uint64_t x, uint64_t n) {
    return (mul_mod(x, x, n) + n - 1) % n;
}

/* trial division primality test (suitable for the small numbers we encounter) */
static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return false;
    }
    return true;
}

/* returns true if the standard Pollard‑rho algorithm with fixed seed fails to
   find a non‑trivial factor of the composite n */
static bool pollard_rho_fails(uint64_t n) {
    uint64_t x = 2 % n;
    uint64_t y = x;
    uint64_t d = 1;
    const int MAX_ITER = 10000;   /* safe upper bound for small n */
    for (int i = 0; i < MAX_ITER && d == 1; ++i) {
        x = f_mod(x, n);
        y = f_mod(f_mod(y, n), n);
        uint64_t diff = x > y ? x - y : y - x;
        d = gcd_uint64(diff, n);
    }
    /* d == n or d == 1 means the algorithm did not find a proper factor */
    return (d == n) || (d == 1);
}

/* dynamic array for the sequence values */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t last_checked = 3;   /* numbers below 4 are not considered */

/* ensure capacity for at least 'need' elements */
static void ensure_cap(size_t need) {
    if (need <= seq_cap) return;
    size_t newcap = seq_cap == 0 ? 128 : seq_cap * 2;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_terms, newcap * sizeof(seq_t));
    seq_terms = tmp;
    seq_cap = newcap;
}

/* the required sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */
    if (seq_len >= (size_t)n) return seq_terms[n - 1];

    while (seq_len < (size_t)n) {
        for (;;) {
            ++last_checked;
            if (last_checked < 4) continue;
            if (is_prime_small(last_checked)) continue;
            if (!pollard_rho_fails(last_checked)) continue;  /* factorable */
            /* store the composite that Pollard‑Rho fails to factor */
            ensure_cap(seq_len + 1);
            seq_terms[seq_len++] = (seq_t)last_checked;
            break;
        }
    }
    return seq_terms[n - 1];
}
