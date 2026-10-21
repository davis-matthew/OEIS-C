/*
 * A291293  Sequence mod 5 defined by Baldini-Eschgfäller coupled dynamical
 *          system (f,lambda,alpha) with f(k) = A000203(k)-1, lambda(y) = 3y+2
 *          mod 5 for y in Y = {0,1,2,3,4}, and alpha(k) = k mod 5 for k in
 *          Omega = {primes}.
 * https://oeis.org/A291293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A291293
 * Definition:
 *   f(k)   = σ(k) – 1  (σ = sum of divisors)
 *   λ(y)   = 3*y + 2 (mod 5)
 *   α(k)   = k mod 5
 *   a(n)   = α(n)          if n is prime
 *           = λ(a(f(n)))   otherwise
 * Only values 0…4 occur.
 */

#include <stdlib.h>   /* malloc, realloc, abort, memset */
#include <stdint.h>   /* uint8_t, size_t */
#include <stdbool.h>  /* bool */
#include <string.h>   /* memset */

#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

static uint8_t *cache_vals = NULL;   /* 0–4 values, 0xFF = unknown */
static size_t   cache_cap  = 0;       /* allocated size */

/* Ensure cache can hold index n (0‑based). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* relocatable memory for both C and C++ */
    uint8_t *tmp = (uint8_t *)realloc((void *)cache_vals, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    cache_vals = tmp;
    memset(cache_vals + cache_cap, 0xFF, new_cap - cache_cap);
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined below offset */

    ensure_cache_size(n);
    if (cache_vals[n] != 0xFF) return (seq_t)cache_vals[n];

    seq_t val;
    if (is_prime(n)) {
        val = (seq_t)n % (seq_t)5;       /* α(n) */
    } else {
        seq_t sigma_minus1 = sigma_of(n) - (seq_t)1;   /* f(n) */
        idx_t f_idx = (idx_t)sigma_minus1;
        seq_t a_fk = a(f_idx);                   /* a(f(n)) */
        val = (3 * a_fk + 2) % (seq_t)5;          /* λ applied */
    }

    cache_vals[n] = (uint8_t)val;
    return val;
}
